#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	// Set variables to pass to fragment
	TexCoord 	= aTexCoord;
	
	vec3 Location = aPos * 0.25;
	gl_Position = vec4(Location, 1.0);
}