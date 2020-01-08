project('libnet', 'c',
        license: 'MIT',
        version: '0.1.0')

subdir('include/net')

net_inc = include_directories('include')
net_src = files('lib/net.c')

net_lib = library('net', net_src, include_directories: [net_inc])
net_dep = declare_dependency(include_directories: [net_inc], link_with: [net_lib])

subdir('test')
