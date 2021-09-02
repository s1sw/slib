slib_src = [
  'Path.cpp',
  'String.cpp',
  'BufferReader.cpp',
  'Win32Util.cpp'
]

slib_inc = include_directories('../include')

slib = static_library('slib', slib_src, include_directories: [slib_inc])

slib_dep = declare_dependency(link_with: slib, include_directories: slib_inc)
