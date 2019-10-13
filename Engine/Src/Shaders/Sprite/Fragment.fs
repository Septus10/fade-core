#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

uniform float nx;
uniform float ny;

uniform float Column;
uniform float Row;

void main()
{
	vec2 UV = vec2(nx * Column + TexCoord.x * nx, ny * Row + TexCoord.y * ny);
	
	FragColor = texture(tex, UV);
	//FragColor = vec4(nx, ny, 0, 1);
}