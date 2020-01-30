# This file adds all of the subdirectories so they can define tests
test_files = []
test_deps = {}

# Use criterion as a dependency for tests. This is defined here to
# allow us to use it in subdirectories
criterion_dep = dependency('criterion', version : '>=2.3.2', required : false)
has_criterion = criterion_dep.found()

# Generic test dependencies. These can be used on any test executable
# to be sure they are linked with the proper dependencies
test_deps += {'base': net_dep}
if has_criterion
    test_deps += {'criterion': [net_dep, criterion_dep]}
endif

# The main test directory. This is platform independent, and just
# confirms the basic top-level functionality of the API
subdir('net')

if posix
    # Posix tests. These are tests specifically aimed at Posix platforms
    # to ensure the compatibility with the Posix specific functions
    subdir('posix')
endif

# Setup all of the executables and tests
foreach file : test_files
    name = file.get('name')
    src = file.get('src')
    type = file.get('type', 'base')
    args = file.get('args', [])
    dep = test_deps.get(type)

    exe = executable('test-' + name, src, dependencies: test_deps.get(type))
    test(name, exe, args: args)
endforeach

# Valgrind test setup
valgrind = find_program('valgrind', required: false)
if valgrind.found()
    always_valgrind = get_option('always_valgrind')
    add_test_setup('valgrind', exe_wrapper:
            [valgrind,
            '--leak-check=full',
            '--trace-children=yes',
            '--errors-for-leak-kinds=all',
            '--error-exitcode=3'],
        timeout_multiplier: 100, is_default: always_valgrind)
    
    if always_valgrind
        message('Using valgrind as the test setup to check for memory leaks')
    else
        message('Valgrind was found, but won\'t be used as the default test setup. To use it, you must enable it manually or setup the project again.')
    endif
else
    message('Valgrind was not found, so it won\'t be used for tests')
endif
