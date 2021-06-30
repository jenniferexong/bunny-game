directories = [
    'engine',
    'game',
]

# visit each directory
foreach dir : directories
    subdir(dir)
endforeach

all_src = [
    engine,
        environment,
        models,
        post_processing,
			post_processing_shaders,
        renderers,
        scene,
        shaders,
        text,
        ui,
        util,
    game,
      game_manager,
      game_scene,
]

opengl = dependency('OpenGL')
deps = [opengl, glm, glew, glfw]

executable(
    'run',
    all_src,
    include_directories : inc,
    dependencies : deps,
    install: true
)
