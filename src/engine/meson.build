engine = files(
    'Application.cpp',
    'IOEvents.cpp',
    'FrustumBox.cpp',
    'Loader.cpp',
    'stb_image.cpp',
    'Frustum.cpp',
    'ShadowBox.cpp',
    'Timer.cpp',
    'Camera.cpp',
)

engine_directories = [
    'environment',
    'models',
    'post-processing',
    'renderers',
    'scene',
    'shaders',
    'text',
    'ui',
    'util'
]

foreach dir: engine_directories
    subdir(dir)
endforeach
