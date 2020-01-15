if posix or (compiler.has_header('unistd.h') and compiler.has_function('fork'))
    aio_forked = executable('test-aio-forked', 'forked.c', dependencies: [net_dep])
    test('aio-forked', aio_forked)
else
    warning('A Posix compliant system was not found. Not running the all-in-one forked test.')
endif

pthread_dep = dependency('threads', required: false)

if pthread_dep.found()
    aio_threaded = executable('test-aio-threaded', 'threaded.c', dependencies: [pthread_dep, net_dep])
    test('aio-threaded', aio_threaded)
else
    warning('Threads do not work or the header is not installed. Not running the all-in-one threaded test.')
endif
