project('orpheus', 'cpp', default_options: [
  'default_library=static',
  'cpp_std=c++20',
  'b_coverage=true',
  'warning_level=3',
], version: '0.1.0')

include_directories = [
  'include',
]

sources = [
  'src/orpheus.cpp'
]

run_target('lint', command: 'tools/lint.sh')
run_target('auto-lint', command: 'tools/auto-lint.sh')

library('orpheus', sources, include_directories: include_directories)

test_includes = [
  include_directories,
  'lib/Catch2/single_include',
  'lib/asciichart/include/ascii',
]

test_sources = [
  ['test_sinesource', 'test/test_sinesource.cpp'],
  ['test_attenuator', 'test/test_attenuator.cpp'],
  ['test_inverse', 'test/test_inverse.cpp'],
  ['test_sum', 'test/test_sum.cpp'],
  ['test_util', 'test/test_util.cpp'],
]

foreach test_src : test_sources
  t_exe = executable(test_src[0], [test_src[1], sources], include_directories: test_includes)
  test(test_src[0], t_exe)
endforeach
