#include <stdlib.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#include <check.h>
#pragma clang diagnostic pop
#include "../src/vault.h"
#include "../src/vault_internal.h"


START_TEST(test_url_token_set_on_init) {
    VaultClient* client = vault_client_new(
        "https://vault.example.com:8200",
        "supersecret"
    );
    ck_assert_str_eq(client->url, "https://vault.example.com:8200");
    ck_assert_str_eq(client->token, "supersecret");
    vault_client_free(client);
}
END_TEST

START_TEST(test_init_with_null_values) {
    VaultClient* client = vault_client_new(
        NULL,
        NULL
    );
    vault_client_free(client);
}
END_TEST

Suite*
client_suite(void) {
    Suite* suite = suite_create("Vault Client");
    TCase* test_case = tcase_create("Constructor");
    tcase_add_test(test_case, test_url_token_set_on_init);
    tcase_add_test(test_case, test_init_with_null_values);
    suite_add_tcase(suite, test_case);
    return suite;
}

int main(void) {
    int number_failed;
    SRunner* runner = srunner_create(client_suite());
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
