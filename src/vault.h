typedef struct _VaultClient VaultClient;

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
