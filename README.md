# libvault - A C library to access the [Vault](https://vaultproject.io) API

[![Build Status](https://travis-ci.org/tbartelmess/libvault.svg?branch=master)](https://travis-ci.org/tbartelmess/libvault)

⚠️ *This is still a work in progress*

Libvault is using [libcurl](https://curl.haxx.se) fo Network requests and [Jansson](https://jansson.readthedocs.io) for JSON parsing.


## Building

```
cmake .
make
```

## Tests
Tests are written using the [Check library](https://github.com/libcheck/check). To run tests build using cmake and `make test`

# Usage

### Creating a new `VaultClient`

The `VaultClient` encapsulates the information needed to connect to a VaultServer. It is required for all vault operations.

```c
#include <vault/vault.h>

// Create a new Vault Client with a token.
VaultClient* client = vault_client_new("https://vault.example.com:8200", "vault_token");

// Create a new Vault Client without a token
VaultClient* client = vault_client_new("https://vault.example.com:8200", NULL);

// Cleanup the Vault Client
vault_client_free(client);
```


### Raw Reading

Using `vault_read` it's easy to read values at a specific vault path. The `VaultValues` struct and get `vault_values_` functions are simple wrappers are around the jansson `json_t` structure.

If only one value is required, the `vault_read_*` function directly return a specific key.

```c
#include <stdio.h>
#include <vault/vault.h>

VaultValues* values = vault_read(client, "secret/password");
const char* value = vault_values_get_string("value");
free(values);
printf("Value: %s\n", value);
free(value)

const char* password = vault_read_string("client", "secret/password", "value");
printf("Password: %s\n", password);

```

### Check Vault Health

Checks the health of the Vault Server. See the [Vault Documentation](https://www.vaultproject.io/docs/http/sys-health.html) for the Health endpoint for more details.

```c
#include <stdio.h>
#include <vault/vault.h>

const VaultHealth* health = vault_health(client);
printf("Cluster Name: %s\n", vault_health->cluster_name);
printf("Cluster ID: %s\n", vault_health->cluster_id);
printf("Vault Version: %s\n" vault_health->version);
printf("Standby: %s\n", vault_health->standby ? "YES", "NO");
printf("Sealed: %s\n", vault_health->sealed ? "YES", "NO");
printf("Initialized: %s\n", vault_health->initialized ? "YES", "NO");

```