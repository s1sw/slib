slib_src = [
  'Path.cpp',
  'String.cpp',
  'BufferReader.cpp',
  'Win32Util.cpp',
  'DynamicLibraryLinux.cpp',
  'DynamicLibraryWin32.cpp',
  'SubprocessWin32.cpp'
]

slib_inc = include_directories('../include')

add_project_arguments('/await', language: 'cpp')
slib = static_library('slib', slib_src, include_directories: [slib_inc], override_options: ['cpp_std=vc++latest'])

slib_dep = declare_dependency(link_with: slib, include_directories: slib_inc)
