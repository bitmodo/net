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
