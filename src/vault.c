#include <stdlib.h>
#include <string.h>

#include "vault.h"
#include "vault_internal.h"

VaultClient*
vault_client_new(const char* url, const char* token) {
    VaultClient* client = malloc(sizeof(VaultClient));
    client->url = strcpy(malloc(strlen(url)+1), url);
    client->token = strcpy(malloc(strlen(token)+1), token);
    return client;
}

void
vault_client_free(VaultClient* client) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-qual"
    if (client->url) free((void*)client->url);
    if (client->token) free((void*)client->token);
#pragma clang diagnostic pop
    free(client);
}
