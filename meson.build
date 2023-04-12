project('orpheus', 'cpp', default_options: [
  'default_library=static',
  'cpp_std=c++20',
], version: '0.1.0')

include_directories = [
  'include'
]

sources = [
  'src/orpheus.cpp'
]

run_target('lint', command: 'tools/lint.sh')
run_target('auto-lint', command: 'tools/auto-lint.sh')

library('orpheus', sources, include_directories: include_directories)

test_catch2_dep = dependency('catch2', main: true, required: false, disabler: true)
test('test_orpheus',
  executable(
    'test_orpheus',
    ['test/test_orpheus.cpp', 'src/orpheus.cpp'],
    include_directories: [include_directories, 'lib/asciichart/include/ascii'],
    dependencies: [test_catch2_dep]
  ),
  args: ['-s']
)
