#include "vault.h"
#include <jansson.h>

typedef struct {
    long http_status;
    json_t* json;
} VaultResponse;

void vault_response_free(VaultResponse* response);

/*!
 Create the URL for a vault endpoint

 @param[in] client VaultClient of the endpoint
 @param[in] endpoint API endpoint (including the version prefix)

*/
const char*
vault_endpoint(VaultClient* client, const char* endpoint);

VaultResponse*
vault_request(VaultClient*, const char* endpoint);
