slib_src = [
  'Path.cpp',
  'String.cpp'
]

slib_inc = include_directories('../include')

slib = library('slib', slib_src, include_directories: [slib_inc])

slib_dep = declare_dependency(link_with: slib, include_directories: slib_inc)
