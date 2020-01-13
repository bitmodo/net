pthread_dep = dependency('threads', required: false)

if pthread_dep.found()
    e = executable('test-basic', 'main.c', dependencies: [pthread_dep, net_dep])
    test('basic', e)
else
    warning('Threads do not work or the header is not installed. Not running the basic test.')
endif
