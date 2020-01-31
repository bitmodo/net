if has_criterion
    args = ['--verbose', '-j1']
    
    test_files += {'name': 'c-posix-initialize', 'type': 'criterion', 'args': args, 'src': files('initialize.c')}
    test_files += {'name': 'c-posix-loop', 'type': 'criterion', 'args': args, 'src': files('loop.c')}
    test_files += {'name': 'c-posix-receive', 'type': 'criterion', 'args': args, 'src': files('receive.c')}
    test_files += {'name': 'c-posix-close-connection', 'type': 'criterion', 'args': args, 'src': files('close_connection.c')}
    test_files += {'name': 'c-posix-close', 'type': 'criterion', 'args': args, 'src': files('close.c')}
endif
