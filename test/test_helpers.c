#include <stdlib.h>
#include "test_helpers.h"


uint32_t success_count = 0;
uint32_t failed_count = 0;

void start_test() {
    printf("Starting: %s\n", __FUNCTION__);
}


void start_suite(const char* name) {

}

void finish_test() {

}

int finish_suite() {
    printf("\n========================\n");
    printf("TEST SUMMARY\n");
    printf("Passed assertions: %d\n", success_count);
    printf("Failed assertions: %d\n", failed_count);
    printf("========================\n");
    return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
