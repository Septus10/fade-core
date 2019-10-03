#include "Texture.hpp"

#define GLEW_STATIC 1

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GL/GL.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace Fade {
namespace Rendering {

CTexture::CTexture(fstl::String a_TexturePath)
{
	LoadTexture(a_TexturePath);
}

bool CTexture::LoadTexture(fstl::String a_TexturePath)
{
	u32 format;
	Fade::uc8* imageData = getImageDataFromPath(a_TexturePath, format);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, imageData);
	//glGenerateMipmap(GL_TEXTURE_2D);

	freeImageData(imageData);

	return true;
}

Fade::uc8* CTexture::getImageDataFromPath(fstl::String a_Filepath, Fade::u32& a_OutFormat)
{
	stbi_set_flip_vertically_on_load(1);
	Fade::i32 channels;
	Fade::uc8* imageData = stbi_load(a_Filepath.c_str(), &m_Width, &m_Height, &channels, 0);
	if (!imageData)
	{
		std::cout << "Unable to load image:\n\t" << stbi_failure_reason() << "\n";
		return nullptr;
	}

	switch (channels)
	{
	case 3:
		a_OutFormat = GL_RGB;
		break;
	case 4:
		a_OutFormat = GL_RGBA;
		break;
	default:
		a_OutFormat = GL_RGB;
		break;
	}

	return imageData;
}

void CTexture::freeImageData(Fade::uc8* a_Data)
{
	stbi_image_free(a_Data);
}

} }