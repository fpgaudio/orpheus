project('orpheus', 'cpp', default_options: [
  'default_library=static',
  'cpp_std=c++20',
  'b_coverage=true',
  'warning_level=3',
], version: '0.1.0')

doxygen = find_program('doxygen', required: false, disabler: true)
if not doxygen.found()
  warning('doxygen not found. Documentation will not be built.')
endif

include_directories = [
  'include',
]

sources = [
  'src/orpheus.cpp'
]

run_target('lint', command: 'tools/lint.sh')
run_target('auto-lint', command: 'tools/auto-lint.sh')

liborpheus = library('orpheus', sources, include_directories: include_directories)
liborpheus_dep = declare_dependency(include_directories: include_directories,
  link_with: liborpheus)

# Testing
###############################################################################
catch2_dep = dependency('catch2')
asciichart_proj = subproject('asciichart')
asciichart_dep = asciichart_proj.get_variable('asciichart_dep')

test_deps = [catch2_dep, asciichart_dep]

test_includes = [include_directories]

test_sources = [
  ['test_sinesource', 'test/test_sinesource.cpp'],
  ['test_attenuator', 'test/test_attenuator.cpp'],
  ['test_inverse', 'test/test_inverse.cpp'],
  ['test_sum', 'test/test_sum.cpp'],
  ['test_util', 'test/test_util.cpp'],
]

foreach test_src : test_sources
  t_exe = executable(
    test_src[0],
    [test_src[1], sources],
    include_directories: test_includes,
    dependencies: test_deps
  )
  test(test_src[0], t_exe)
endforeach

doc_config = configuration_data()
doc_config.set('PACKAGE_NAME', meson.project_name())
doc_config.set('PACKAGE_VERSION', meson.project_version())
doc_config.set('builddir', meson.current_build_dir())
doc_config.set('srcdir', meson.source_root())

doxyfile = configure_file(input: 'docs' / 'orpheus.doxygen.in',
                          output: 'orpheus.doxygen',
                          configuration: doc_config,
                          install: false)

doxy_src = [
  'docs' / 'mainpage.dox',
  'include' / 'orpheus.hpp',
]

custom_target(
  'docs',
  input: doxy_src,
  output: ['.'],
  command: [doxygen, doxyfile],
  install: false,
  build_by_default: true
)
