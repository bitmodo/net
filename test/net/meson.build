get_handler_e = executable('test-get-handler', 'get_handler.c', dependencies: [net_dep])
test('get-handler', get_handler_e)

set_handler_e = executable('test-set-handler', 'set_handler.c', dependencies: [net_dep])
test('set-handler', set_handler_e)

cleanup_e = executable('test-cleanup', 'cleanup.c', dependencies: [net_dep])
test('cleanup', cleanup_e)

set_address_e = executable('test-set-address', 'set_address.c', dependencies: [net_dep])
test('set-address', set_address_e)
