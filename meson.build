project(
  'redhand',
  'c',
  'cpp',
default_options : ['c_std=c11', 'cpp_std=c++17', 'b_lto=True'],
version : '0.1.3',
license : 'LGPL-v3'
)

cc = meson.get_compiler('cpp')
ssmod = import('sourceset')
ss = ssmod.source_set()

pymod = import('python')

python_string = 'python3'
if build_machine.system() == 'windows'
    python_string = 'python'
endif

py = pymod.find_installation(python_string, required: false, modules: ['glad'])
if not py.found()
    pip = pymod.find_installation(python_string, required: true, modules: ['pip'])
    r = run_command(pip.path(), '-m', 'pip', 'install', '--user', 'glad')
    if r.returncode() != 0
        error('error while installing glad')
    endif
    
    ninja = find_program('ninja')
    if not ninja.found()
        run_command(pip.path(), '-m', 'pip', 'install', '--user', 'ninja')
    endif
    
    py = pymod.find_installation(python_string, required: true, modules: ['glad'])
endif

deps = [
    dependency('threads', required : true),
    dependency('glfw3', required : true, fallback : ['glfw3', 'glfw_dep']),
    dependency('gl', required : true),
    dependency('glm', required : true, fallback : ['glm','glm_dep']),
    dependency('stb', required : true, fallback : ['stb', 'stb_dep']),
    py.dependency(),
    cc.find_library('dl', required : false),
]

audio_deps = [
    dependency('openal', required : false),
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

ss.add(
    when: deps,
    if_true: files([
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
    ])
)

#ss.add(when: audio_deps, if_true: files('src/audio.cpp'))

redhand = library(
    'redhand', 
    ss.all_sources(), 
    version : meson.project_version(), 
    soversion : '0',
    include_directories : incdir,
    dependencies: ss.all_dependencies()
)

redhand_dep = declare_dependency(
    include_directories : include_directories('include'),
    link_with : redhand,
    dependencies : ss.all_dependencies(),
    version: meson.project_version()
)

install_subdir('include/redhand', install_dir : 'include')

pkg = import('pkgconfig')
pkg.generate(redhand)

if meson.version().version_compare('>=0.54.0')
    meson.override_dependency('redhand', redhand_dep)
    meson.override_dependency('libredhand', redhand_dep)
endif

if get_option('build-testgame')
    testgame_proj = subproject('redhand-testgame')
    testgame_dep = dependency('redhand', required: true)
    
    executable(
        'redhand-testgame', 
        testgame_proj.get_variable('testgame_sources'),
        include_directories : testgame_proj.get_variable('testgame_incdir'),
        dependencies: testgame_dep,
    )
endif
