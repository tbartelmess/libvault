#include "vault_internal.h"
#include "vault_request.h"

#include <stdlib.h>
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
        printf("Failed to allocate memory for URL\n");
        exit(1);
    }
    sprintf(url, "%s%s", client->url, endpoint);
    return url;
}
