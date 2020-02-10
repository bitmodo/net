#include "tests.h"

int main() {
    setup();

    assert(netStart(NULL) == ENULL_POINTER && "Did not receive the expected value for a null socket");

    cleanup();
}
