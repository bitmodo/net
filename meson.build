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
net_src = files('lib/net.c')

# Add source files depending on the compile system
system = build_machine.system()
has_platform = false
if system == 'windows'
    #net_src += files('')
    error('Windows is not yet supported')
elif system == 'darwin'
    #net_src += files('')
    error('Darwin is not yet supported')
endif

if not has_platform
    if not compiler.has_header('stdio.h') or not compiler.has_header('stdbool.h') or not compiler.has_header('sys/socket.h') or not compiler.has_header('netinet/in.h') or not compiler.has_header('arpa/inet.h') or not compiler.has_header('netdb.h') or not compiler.has_header('unistd.h') or not compiler.has_header('string.h') or not compiler.has_header('errno.h')
        warning('No platform is compiled for. If this is not what you want, check your environment.')
    else
        net_src += files('lib/platform/posix.c')
        message('Compiling for POSIX since that is the only platform this system supports')
    endif
endif

# Library stuff & headers
install_headers(files('include/net/net.h', 'include/net/error.h', 'include/net/util.h'), subdir: 'net')
net_lib = library('net', net_src, include_directories: [net_inc], install: true)
net_dep = declare_dependency(include_directories: [net_inc], link_with: [net_lib])

# Add pkgconfig stuff
pkg = import('pkgconfig')
libs = [net_lib]
h = ['.', 'include/net']
pkg.generate(libraries : libs,
             subdirs : h,
             version : meson.project_version(),
             name : meson.project_name(),
             filebase : meson.project_name(),
             description : 'A networking library.')

# Setup all of the tests
subdir('test')
