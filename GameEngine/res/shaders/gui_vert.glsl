#version 330 core 

uniform mat4 uTransformationMatrix;

layout(location = 0) in vec2 aPosition;

out VertexData {
    vec2 textureCoords;
} v_out; 

void main(void){
	gl_Position = uTransformationMatrix * vec4(aPosition, 0.0, 1.0);

    // convert to uv coordinate
	v_out.textureCoords = vec2((aPosition.x + 1.0) / 2.0, 1 - (aPosition.y + 1.0) / 2.0);
}