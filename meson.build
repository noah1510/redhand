project(
  'redhand',
  'c',
  'cpp',
default_options : ['c_std=c11', 'cpp_std=c++17', 'b_lto=True'],
version : '0.1.3',
license : 'LGPL-v3'
)

cc = meson.get_compiler('cpp')

pymod = import('python')
py = pymod.find_installation('python3', required: true, modules: ['glad'])

deps = [
    dependency('threads', required : true),
    dependency('glfw3', required : true, fallback : ['glfw3', 'glfw_dep']),
    dependency('gl', required : true),
    dependency('vips', required : true),
    dependency('vips-cpp', required : true),
    dependency('glib-2.0', required : true, fallback : ['glib','libglib_dep']),
    dependency('gobject-2.0', required : true, fallback : ['glib','libgobject_dep']),
    dependency('gio-2.0', required : true, fallback : ['glib','libgio_dep']),
    dependency('openal', required : true),
    py.dependency(),
    cc.find_library('dl', required : false),
]

glad_args = [
    '--generator=c',
    '--extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic',
    '--out-path=.',
    '--reproducible',
    '--profile',
    'core',
]

r = run_command(py.path(), '-m', 'glad', glad_args)
if r.returncode() != 0
  message('error while executing glad')
endif
output = r.stdout().strip()
errortxt = r.stderr().strip()

run_command('rm', '-rf', 'include/redhand/KHR', 'include/redhand/glad')
run_command('mv', '-f', 'include/glad', 'include/redhand/glad')
run_command('mv', '-f', 'include/KHR', 'include/redhand/KHR')

incdir = include_directories('include', 'include/redhand')

sources = [
    'src/actor.cpp', 
    'src/complex_world.cpp', 
    'src/engine.cpp', 
    'src/game_object.cpp', 
    'src/glad.c', 
    'src/helper.cpp', 
    'src/input.cpp', 
    'src/shader.cpp', 
    'src/texture.cpp', 
    'src/types.cpp'
]

redhand = library(
    'redhand', 
    sources, 
    version : meson.project_version(), 
    soversion : '0',
    include_directories : incdir,
    dependencies: deps
)

redhand_dep = declare_dependency(
    include_directories : include_directories('include'),
    link_with : redhand,
    dependencies : deps,
    version: meson.project_version()
)

if get_option('build-testgame')
    testgame_proj = subproject('redhand-testgame')
    
    executable(
        'redhand-testgame', 
        testgame_proj.get_variable('testgame_sources'),
        include_directories : testgame_proj.get_variable('testgame_incdir'),
        dependencies: redhand_dep
    )
endif

install_subdir('include/redhand', install_dir : 'include')

pkg = import('pkgconfig')
pkg.generate(redhand)
