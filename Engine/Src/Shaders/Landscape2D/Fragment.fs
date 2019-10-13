#version 450 core
out vec4 FragColor;

in vec2 UV;
flat in vec2 tilemapUV;

uniform sampler2D Tileset;
uniform sampler2D Tilemap;

uniform float ChunkWidthUnits;
uniform float ChunkHeightUnits;
uniform float HorizontalTiles;
uniform float VerticalTiles;

uniform vec2 TilesetResolution;
uniform vec2 TileResolution;

void main()
{
	vec4 setColor = texture(Tilemap, tilemapUV);

	//vec2 mapCoord = setColor.rg * 255;
	vec2 mapCoord = tilemapUV * 16;
	//vec2 mapCOord = vec2(1, 0);

	vec2 invertedUV = vec2(0) - UV;
	vec2 halfTexelUV = vec2(1.0f / TilesetResolution) * 0.5f;
	vec2 tileDeltaUV = (TileResolution / TilesetResolution);
	vec2 tilesetUV = tileDeltaUV * mapCoord + tileDeltaUV * UV + halfTexelUV * invertedUV;
	//multisample_texel(tilesetUV, texelUV, FragColor);
	FragColor = texture(Tileset, tilesetUV);
}