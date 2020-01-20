#include "net/net.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

bool cleaned = false;

void customCleanup() {
    cleaned = true;
}

int main() {
    gHandler = NULL;
    netCleanup();
    assert(gHandler == NULL && "Handler was not null after cleanup");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    netCleanup();
    assert(gHandler == NULL && "Handler was not null after setting it then cleanup");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &customCleanup};
    netCleanup();
    assert(gHandler == NULL && "Handler was not null after setting it then cleanup");
    assert(cleaned == true && "Cleanup function was not called whenever cleaning up");

    exit(EXIT_SUCCESS);
}
