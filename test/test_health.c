#include <stdlib.h>
#include "../src/vault.h"
#include "run_vault.h"
#include "test_helpers.h"


DELCARE_SUITE(health)
DELCARE_SUITE(reading)


int main() {
  suite_health();
  suite_reading();
  return EXIT_SUCCESS;
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

BEGIN_TEST(read_json)

END_TEST



DEFINE_SUITE(reading)
END_SUITE
