project('libnet', 'c',
        license: 'MIT',
        version: '0.1.0')

# Add version configuration
subdir('include/net')

net_inc = include_directories('include')
net_src = files('lib/net.c',
                'lib/platform/platform.c')

# Add source files depending on the compile system
system = build_machine.system()
if system.to_upper() == 'WINDOWS'
    #net_src += files('')
elif system.to_upper() == 'APPLE'
    #net_src += files('')
elif system.to_upper() == 'LINUX'
    net_src += files('lib/platform/linux.c')
endif

net_lib = library('net', net_src, include_directories: [net_inc])
net_dep = declare_dependency(include_directories: [net_inc], link_with: [net_lib])

# Setup all of the tests
subdir('test')
