#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform float xPos;

out vec2 TexCoord;

void main()
{
	vec3 Location = aPos;
	Location.x += xPos;
	TexCoord = aTexCoord;
	gl_Position = vec4(Location, 1.0);
}