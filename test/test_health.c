#include <stdlib.h>
#include "../src/vault.h"
#include "run_vault.h"
#include "test_helpers.h"


DELCARE_SUITE(health)

int main() {
  suite_health();
}

BEGIN_TEST(health_result)

    const VaultHealth* health = vault_health(client);
    vault_assert(health);

    // The dev server should not be in standby
    vault_assert(!health->standby);
    
    // The dev server automatically unseals
    vault_assert(!health->sealed);

    // The dev server should be automatically initialized
    vault_assert(health->initialized);


END_TEST

DEFINE_SUITE(health)
  RUN_TEST(health_result);
END_SUITE
