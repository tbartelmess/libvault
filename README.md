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

// Create a new Vault Client.
VaultClient* client = vault_client_new("https://vault.example.com:8200", "vault_token");
```

