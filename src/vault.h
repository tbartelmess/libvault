#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

typedef struct _VaultClient VaultClient;

#define VAULT_OK         0
#define VAULT_ERR_ALLOC  1

/*!
 Create a new Vault Client with an Address and an optional vault token

 @param[in] url URL of the Vault
 @param[in] token token

 @return A new vault client.
*/
VaultClient*
vault_client_new(const char* url, const char* token);

/**
 Free a VaultClient

 @param [in] client VaultClient to be freed
*/
void
vault_client_free(VaultClient* client);


/** @section Vault Health */

typedef struct {
    const char* cluster_name;
    const char* cluster_id;
    const char* version;
    const time_t time;

    bool standby;
    bool sealed;
    bool initialized;
    uint8_t __padding[5];
} VaultHealth;

void vault_health_free(VaultHealth* health);


/**
 Query the Vault Server for health

 @param [in] client VaultClient

 @return On success a VaultHealth with the vault health data is retruned.
         Use use vault_health_free to free VaultHealth when you no longer
         need it.
         On failure NULL is returned.
 */
const VaultHealth*
vault_health(VaultClient*client);

