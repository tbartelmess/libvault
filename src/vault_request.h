#include "vault.h"

/*!
 Create the URL for a vault endpoint

 @param[in] client VaultClient of the endpoint
 @param[in] endpoint API endpoint (including the version prefix)

*/
const char*
vault_endpoint(VaultClient* client, const char* endpoint);
