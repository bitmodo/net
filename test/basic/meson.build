compiler = meson.get_compiler('c')
if compiler.has_header('threads.h') and compiler.has_function('thrd_create') and compiler.has_function('thrd_join')
    e = executable('test-basic', 'main.c', dependencies: [net_dep])
    test('basic', e)
else
    warning('Threads do not work or the header is not installed. Not running the basic test.')
endif
