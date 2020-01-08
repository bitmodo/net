#include <stdlib.h>
#include "net/net.h"
#include "platform.h"

#define SETUP(val)           \
NetSocket NetSetupSocket() { \
    return val;              \
}

#if defined(_WIN32) || defined(_WIN64)
#error "Windows is not supported yet"
#elif defined(__APPLE__)
#error "Apple is not supported yet"
#elif defined(__linux__)
SETUP(NetLinuxSetup())
#else
SETUP(NULL)
#endif
