#version 330 core

layout(location = 0) in vec2 aPosition;

out VertexData {
    vec2 textureCoords;
} v_out;

void main() 
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    v_out.textureCoords = (aPosition + 1.0) / 2.0;
}
