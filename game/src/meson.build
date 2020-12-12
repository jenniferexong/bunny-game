directories = [
    'engine',
    'environment',
    'game-manager',
    'pkg',
    'scene',
    'ui',
    'util',
]

# visit each directory
foreach dir : directories
    subdir(dir)
endforeach

src = ['Main.cpp']

all_src = [
    src,
    engine,
        models,
        post_processing,
        renderers,
        shaders,
        text,
    environment,
    game_manager,
    pkg,
    scene,
    ui,
    util,
]

opengl = dependency('OpenGL')
deps = [opengl, glm, glew, glfw]

executable(
    'game',
    all_src,
    include_directories : inc,
    dependencies : deps,
    install: true
)
