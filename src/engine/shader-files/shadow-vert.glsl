#version 330 core

layout(location = 0) in vec3 aPosition;

// model, view, projection
uniform mat4 uMatrix; 

void main()
{
	gl_Position = uMatrix * vec4(aPosition, 1.0);
}
