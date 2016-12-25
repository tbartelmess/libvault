#include "vault.h"
#include "vault_internal.h"
#include "vault_request.h"
#include <jansson.h>

#include <string.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#include <curl/curl.h>
#pragma clang diagnostic pop

const char*
vault_endpoint(VaultClient* client, const char* endpoint) {
    size_t url_length = strlen(client->url) + strlen(endpoint);
    char* url = malloc(url_length + 1);
    if (!url) {
        vault_client_set_error(
            client,
            VAULT_ERR_ALLOC,
            "Failed to allocate memory for URL"
        );
        return NULL;
    }
    sprintf(url, "%s%s", client->url, endpoint);
    return url;
}


typedef struct {
    int8_t* data;
    size_t size;
    VaultClient* client;
} VaultResponseData;


uint32_t
vault_response_data_grow(VaultResponseData* data, size_t size);

uint32_t
vault_response_data_grow(VaultResponseData* data, size_t size) {
    if (!data->data) {
        data->data = malloc(size);
    } else {
        data->data = realloc(data->data, data->size + size);
    }
    if (!data->data) {
        vault_client_set_error(
            data->client,
            VAULT_ERR_ALLOC,
            "Failed to alloc response data chunk"
        );
        return VAULT_ERR_ALLOC;
    }
    data->size += size;
    return VAULT_OK;
}


static size_t
vault_response_write_callback(void* contents, size_t size, size_t nmemb, void* userdata) {
    VaultResponseData* response_data = (VaultResponseData*)userdata;
    size_t content_size = size * nmemb;
    uint32_t response = vault_response_data_grow(
        response_data,
        response_data->size + content_size
    );
    if (response != VAULT_OK) {
        return 0;
    }
    memcpy(
        response_data->data + ((response_data->size) - content_size),
        contents,
        content_size
    );
    return content_size;
}

VaultResponse*
vault_request(VaultClient* client, const char* endpoint) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
    CURL *curl = curl_easy_init();
    if (!curl) {
        // TODO: Shit went wrong
        return NULL;
    }
    VaultResponseData* response_data = calloc(1, sizeof(VaultResponseData));
    response_data->client = client;
    curl_easy_setopt(curl, CURLOPT_URL, vault_endpoint(client, endpoint));
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, vault_response_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);
    int result = curl_easy_perform(curl);
    if (result == CURLE_OK) {
        VaultResponse* response = malloc(sizeof(VaultResponse));
        long status_code;
        int curl_res = curl_easy_getinfo(curl,
                                         CURLINFO_RESPONSE_CODE,
                                         &status_code);
        
        response->http_status = status_code;
        char* content_type;
        curl_res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
        if (strcmp(content_type, "application/json") != 0) {
                // Wrong content type
        }

        // Parse json
        json_error_t* error = NULL;
        response->json = json_loadb((const char*)response_data->data,
                                    response_data->size,
                                    (JSON_REJECT_DUPLICATES),
                                    error);

        return response;
    }
    return NULL;
#pragma clang diagnostic pop
}
