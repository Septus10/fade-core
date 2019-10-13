#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec2 aTilemapUV;

uniform float ChunkWidthUnits;
uniform float ChunkHeightUnits;

uniform vec2 CamPos;

out vec2 UV;
flat out vec2 tilemapUV;

void main()
{
	// Set variables to pass to fragment
	UV 				= aUV;
	tilemapUV 		= aTilemapUV;

	gl_Position = vec4(vec2(aPos - CamPos), 0, 500);
}