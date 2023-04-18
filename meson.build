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
  'test/test_sinesource.cpp',
  'test/test_attenuator.cpp',
  'test/test_inverse.cpp',
  'test/test_sum.cpp',
  'test/test_orpheus.cpp',
]

test('test_orpheus',
  executable(
    'test_orpheus',
    [test_sources, 'src/orpheus.cpp'],
    include_directories: test_includes,
  ),
  args: ['-s']
)
