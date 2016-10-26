#include <stdint.h>
#include <stdlib.h>

struct _VaultClient {
    const char* url;
    const char* token;

    uint32_t err_code;
    uint32_t _padding;

    const char* err_string;
};

void* vault_malloc(struct _VaultClient* client,
                   size_t size);

void
vault_client_set_error(struct _VaultClient* client,
                       uint32_t     err_code,
                       const char*  err_string);
