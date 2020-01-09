#include "net/net.h"

#define SETUP_METHOD(name)         \
NetHandler * name();               \
NetHandler * net_setupPlatform() { \
    return name();                 \
}

#if defined(_WIN32) || defined(_WIN64)
#error "Windows is not supported yet"
#elif defined(__APPLE__)
#error "Apple is not supported yet"
#elif defined(__linux)
SETUP_METHOD(net_linux)
#else
#warning "Compiling without a handler implementation because the current platform is not supported. This is only recommended if you are going to write your own implementation."
#endif
