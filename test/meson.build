# This file adds all of the subdirectories so they can define tests

# Use criterion as a dependency for tests. This is defined here to
# allow us to use it in subdirectories
criterion_dep = dependency('criterion', version : '>=2.3.2', required : false)
has_criterion = criterion_dep.found()

# Generic test dependencies. These can be used on any test executable
# to be sure they are linked with the proper dependencies
test_deps = [net_dep]
if has_criterion
    test_deps += criterion_dep
endif

# The main test directory. This is platform independent, and just
# confirms the basic top-level functionality of the API
subdir('net')

if posix
    # Posix tests. These are tests specifically aimed at Posix platforms
    # to ensure the compatibility with the Posix specific functions
    subdir('posix')
endif
