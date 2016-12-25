# libvault - A C library to access the [Vault](https://vaultproject.io) API

[![Build Status](https://travis-ci.org/tbartelmess/libvault.svg?branch=master)](https://travis-ci.org/tbartelmess/libvault)




*This is still a work in progress*

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