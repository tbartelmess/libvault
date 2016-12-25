#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "vault.h"
#include "vault_internal.h"
#include "vault_request.h"

#define FREE_IF_SET(var) if (var) free((void*)var);

VaultClient*
vault_client_new(const char* url, const char* token) {
    VaultClient* client = calloc(1, sizeof(VaultClient));

    if (url) {
        client->url = strcpy(malloc(strlen(url)+1), url);
    }

    if (token) {
        client->token = strcpy(malloc(strlen(token)+1), token);
    }
    return client;
}


void
vault_client_free(VaultClient* client) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-qual"
    FREE_IF_SET(client->url);
    FREE_IF_SET(client->token);
    FREE_IF_SET(client->err_string);
#pragma clang diagnostic pop
    free(client);
}


void
vault_client_set_error(VaultClient* client,
                       uint32_t     err_code,
                       const char*  err_string) {
    client->err_code = err_code;
    if (client->err_string) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-qual"
        free((void *)client->err_string);
#pragma clang diagnostic pop
    }

    if (err_string) {
        char* _err_string = malloc(strlen(err_string) + 1);
        if (!client->err_string) {
            printf("Failed to allocate memory for error message\n");
            abort();
        }
        _err_string = strcpy(_err_string, err_string);
        client->err_string = _err_string;
    }
}

static bool
set_json_string_value(VaultResponse* response,
                           const char* key,
                           const char** dest) {
   json_t* j_value = json_object_get(response->json,
                                     key);
   if (!json_is_string(j_value)) {
        return false;
   }
   
   char* copy = malloc(strlen(json_string_value(j_value)) + 1);
   copy = strcpy(copy, json_string_value(j_value));
   *dest = copy;
   return true;
}

static bool
set_json_bool_value(VaultResponse* response,
                           const char* key,
                           bool* dest) {
    json_t* j_value = json_object_get(response->json,
                                     key);
   if (!json_is_boolean(j_value)) {
        return false;
   }

   *dest = json_boolean_value(j_value);

   return true;

}
#define SET_JSON_STR(resp, key, dest) if (!set_json_string_value(resp, key , dest)) { return NULL; }
#define SET_JSON_BOOL(resp, key, dest) if (!set_json_bool_value(resp, key , dest)) { return NULL; }




void vault_health_free(VaultHealth* health) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-qual"
    FREE_IF_SET(health->cluster_id);
    FREE_IF_SET(health->cluster_name);
    FREE_IF_SET(health->version);
#pragma clang diagnostic pop
    free(health);
}

const VaultHealth*
vault_health(VaultClient* client) {
    VaultResponse* health_response = vault_request(
        client,
        "/v1/sys/health"
    );
    VaultHealth* health = malloc(sizeof(VaultHealth));

    SET_JSON_STR(health_response, "cluster_name", &health->cluster_name)
    SET_JSON_STR(health_response, "cluster_id", &health->cluster_id)
    SET_JSON_STR(health_response, "version", &health->version)

    SET_JSON_BOOL(health_response, "standby", &health->standby);
    SET_JSON_BOOL(health_response, "sealed", &health->sealed);
    SET_JSON_BOOL(health_response, "initialized", &health->initialized);

    return health;


}
