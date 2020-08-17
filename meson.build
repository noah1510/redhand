project(
    'redhand', 
    'c', 
    'cpp',
    default_options : [
        'c_std=c11', 
        'cpp_std=c++17'
    ]
)

#add_project_arguments('-std=gnu++17', language : 'cpp')
cc = meson.get_compiler('cpp')

deps = [
    dependency('threads', required : true),
    dependency('glfw3', required : true),
    dependency('opengl', required : true),
    dependency('vips', required : true),
    dependency('vips-cpp', required : true),
    dependency('glib-2.0', required : true),
    dependency('openal', required : true),
    cc.find_library('dl', required : false),
]

incdir = include_directories('include')

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

library(
    'redhand', 
    sources, 
    version : '0.1.2', 
    soversion : '0', 
    include_directories : incdir,
    dependencies: deps
)
