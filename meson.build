project('libnet', 'c',
        license: 'MIT',
        version: '0.1.0')

# Add version configuration
subdir('include/net')

# Check compiler stuff
compiler = meson.get_compiler('c')
if not compiler.has_header('stdlib.h')
    error('Could not find stdlib.h. Make sure you have your environment properly setup.')
endif

net_inc = include_directories('include')
net_src = files('lib/net.c',
                'lib/platform/platform.c')

# Add source files depending on the compile system
system = build_machine.system()
has_platform = false
if system == 'windows'
    #net_src += files('')
    error('Windows is not yet supported')
elif system == 'darwin'
    #net_src += files('')
    error('Darwin is not yet supported')
elif system == 'linux'
    if not compiler.has_header('stdio.h') or not compiler.has_header('stdbool.h') or not compiler.has_header('sys/socket.h') or not compiler.has_header('netinet/in.h') or not compiler.has_header('arpa/inet.h') or not compiler.has_header('netdb.h') or not compiler.has_header('unistd.h') or not compiler.has_header('string.h') or not compiler.has_header('errno.h')
        error('Could not find all of the necessary headers for linux. If you are compiling for it. Make sure you have your environment properly setup.')
    else
        net_src += files('lib/platform/linux.c')
        has_platform = true
    endif
endif

if not has_platform
    warning('No platform is compiled for. If this is not what you want, check your environment.')
endif

net_lib = library('net', net_src, include_directories: [net_inc], install: true)
net_dep = declare_dependency(include_directories: [net_inc], link_with: [net_lib])

# Setup all of the tests
subdir('test')
