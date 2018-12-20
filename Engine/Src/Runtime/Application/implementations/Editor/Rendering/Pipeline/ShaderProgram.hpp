#pragma once

#include <string>
#include <Core/Containers/Array.hpp>
#include <Core/definitions.hpp>

namespace Fade { namespace Rendering { namespace Pipeline {

#define SHADER_ERROR -1;

enum class EShaderType : u8
{
	Vertex		= 0,
	Fragment	= 1,
	Geometry	= 2,
	Unknown		= 3,
	NUM_SHADERTYPES = 4
};

class CShaderProgram
{
public:
	CShaderProgram();
	CShaderProgram(std::string a_FolderPath);

	int LoadShaderProgram(std::string a_FolderPath);

	void Use();

private:
	int LoadShader(std::string a_FileContents, EShaderType a_ShaderType);

	u32 m_ProgramID;

};

}}}