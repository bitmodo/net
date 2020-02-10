project('libnet', 'c',
        license: 'MIT',
        version: '0.1.0-alpha1')

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
windows = system == 'windows'
darwin = system == 'darwin'
posix = system in ['cygwin', 'darwin', 'dragonfly', 'freebsd', 'linux', 'netbsd', 'openbsd', 'sunos']

if windows
    #net_src += files('')
    error('Windows is not yet supported')
elif darwin
    #net_src += files('')
    error('Darwin is not yet supported')
elif posix
    net_src += files('lib/platform/posix.c')
    message('Compiling for POSIX since that is the only platform this system supports')
else
    warning('No platform is compiled for. If this is not what you want, check your environment.')
endif

net_c_args = []
if compiler.has_argument('-Wno-nonnull-compare')
    net_c_args += '-Wno-nonnull-compare'
endif

# Library stuff & headers
install_headers(files('include/net/net.h', 'include/net/error.h', 'include/net/util.h'), subdir: 'net')
net_lib = library('net', net_src, include_directories: [net_inc], c_args: ['-DNET_COMPILATION'] + net_c_args, install: true)
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

# Setup all of the tests and examples
subdir('test')
subdir('example')
