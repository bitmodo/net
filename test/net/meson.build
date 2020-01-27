if has_criterion
    handler_e = executable('test-handler', 'handler.c', dependencies: test_deps)
    test('handler', handler_e, args: ['--verbose', '-j1'])

    parameters_e = executable('test-parameters', 'parameters.c', dependencies: test_deps)
    test('parameters', parameters_e, args: ['--verbose', '-j1'])
endif
