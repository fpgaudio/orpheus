project('orpheus', 'cpp', default_options: 'default_library=static', version: '0.1.0')

sources = [
  'src/orpheus.cpp'
]

run_target('lint', command: 'tools/lint.sh')
run_target('auto-lint', command: 'tools/auto-lint.sh')

library('orpheus', sources, include_directories: ['include'])
