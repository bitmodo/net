compiler = meson.get_compiler('c')
pthread_dep = dependency('threads', required: false)

# and compiler.has_header('pthread.h') and compiler.has_header('unistd.h') and compiler.has_function('pthread_create') and compiler.has_function('pthread_join') and compiler.has_function('sleep')
if pthread_dep.found()
    e = executable('test-basic', 'main.c', dependencies: [pthread_dep, net_dep])
    test('basic', e)
else
    warning('Threads do not work or the header is not installed. Not running the basic test.')
endif
