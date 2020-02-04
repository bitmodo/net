#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

#include "tests.h"

int main() {
    setup();

    assert(netConnect(NULL) == ENULL_POINTER && "Did not receive the expected value for a null socket");

    cleanup();
}
