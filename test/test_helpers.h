#include <stdio.h>
#include <stdint.h>
#include "run_vault.h"
extern uint32_t success_count;
extern uint32_t failed_count;

void start_test();
void finish_test();

void start_suite(const char* name);
int finish_suite();

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m" 
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_BOLD_ON "\x1b[1m"
#define ANSI_UNDERLINE_ON "\x1b[4m"
#define ANSI_RESET   "\x1b[0m"

#define DELCARE_SUITE(__suitename)\
void suite_##__suitename();

#define DEFINE_SUITE(__suitename)\
void suite_##__suitename() {\
    run_vault();

#define RUN_TEST(__testname) \
    test_##__testname();

#define END_SUITE }



#define BEGIN_TEST(__testname)\
static void test_##__testname();\
static void test_##__testname() {\
    start_test();\
    VaultClient* client = vault_client_new(\
        "http://localhost:8200",\
        "vault_testing"\
    );


#define END_TEST \
    finish_test();\
}


#define vault_assert(test) \
    if (!(test)) {\
        printf("\n\n%s%s⎡ Assertion failed%s: Expected %s%s%s%s ot be true\n"\
               "⎣ Code: %s:%d) failed\n\n", ANSI_BOLD_ON,ANSI_RED,ANSI_RESET, ANSI_UNDERLINE_ON, ANSI_BOLD_ON, #test, ANSI_RESET,  __FILE__, __LINE__);\
        failed_count+=1;\
    } else {\
        success_count+=1;\
        printf("%s.%s", ANSI_GREEN, ANSI_RESET);\
    }

