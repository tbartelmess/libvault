#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vault.h"
#include "vault_internal.h"

VaultClient*
vault_client_new(const char* url, const char* token) {
    VaultClient* client = malloc(sizeof(VaultClient));

    (*client).url = NULL;
    (*client).token = NULL;

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
    if (client->url) free((void*)client->url);
    if (client->token) free((void*)client->token);
    if (client->err_string) free((void*)client->token);
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
