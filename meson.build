project(
    'nibbles',
     'cpp',
     default_options : ['cpp_std=c++20'],
)

glew = dependency('glew')
glfw = dependency('glfw3')
glm = dependency('glm')
# opengl = dependency('OpenGL')

subdir('dependencies')

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
