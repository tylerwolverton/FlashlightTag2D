#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 texCoords;

uniform mat4 model;
uniform mat4 projection;

uniform vec2 textureSize;
uniform vec2 texturePos;

void main()
{
    texCoords = vec2(textureSize.x * vertex.z + texturePos.x, textureSize.y * vertex.w + texturePos.y);
	
    gl_Position = vec4(vertex.xy, 0.0, 1.0) * model * projection;
}