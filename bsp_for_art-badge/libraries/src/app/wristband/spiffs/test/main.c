#include <stdlib.h>

#ifndef NO_TEST
#include "testrunner.h"
#endif

int main(int argc, char **args)
{
    extern uint32_t random_seed_value;
    srand(random_seed_value);
#ifndef NO_TEST
    run_tests(argc, args);
#endif
    exit(EXIT_SUCCESS);
}
