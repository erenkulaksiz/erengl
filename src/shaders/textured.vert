#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
out vec3 color;
out vec2 texCoord;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  FragPos = aPos;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	color = aColor;
	texCoord = aTex;
}