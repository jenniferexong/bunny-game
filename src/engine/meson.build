engine = files(
    'FrustumBox.cpp',
    'Loader.cpp',
    'stb_image.cpp',
    'Frustum.cpp',
    'ShadowBox.cpp',
    'Engine.cpp',
)

engine_directories = [
    'models',
    'post-processing',
    'renderers',
    'shaders',
    'text',
]

foreach dir: engine_directories
    subdir(dir)
endforeach
