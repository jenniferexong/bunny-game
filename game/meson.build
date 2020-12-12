project(
    'nibbles',
     'cpp',
     default_options : ['cpp_std=c++17'],
)

add_global_arguments('-std=c++11', '-std=c++14', '-std=c++17', language : 'cpp')

glew = dependency('glew')
glfw = dependency('glfw3')
glm = dependency('glm')
# opengl = dependency('OpenGL')

subdir('Dependencies')

# include directories
game_inc = include_directories('dependencies')
res_inc = include_directories('res')

inc = [
    game_inc,
    res_inc,
    stb_inc,
]

# all dependencies needed
deps = [
    glew,
    glfw,
    glm
]

subdir('src')