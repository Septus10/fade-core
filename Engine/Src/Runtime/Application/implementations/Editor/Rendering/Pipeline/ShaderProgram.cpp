#include "ShaderProgram.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string> 

#include <GL/glew.h>

namespace std { 
	using namespace experimental;
}

namespace Fade { namespace Rendering { namespace Pipeline {

CShaderProgram::CShaderProgram()
{}

CShaderProgram::CShaderProgram(std::string a_FolderPath)
{
	LoadShaderProgram(a_FolderPath);
}

int CShaderProgram::LoadShaderProgram(std::string a_FolderPath)
{
	// Get all files in folder
	std::filesystem::path Path(a_FolderPath);
	if (!std::filesystem::is_directory(Path))
	{
		std::cout << "ERROR: Path provided is not a directory\n\t" << a_FolderPath << "\n";
		return -1;
	}

	const usize NumShaders = static_cast<usize>(EShaderType::NUM_SHADERTYPES);
	int Shaders[NumShaders];
	std::fill_n(Shaders, NumShaders, -1);
	// Go through all the files in the directory
	for (auto& It: std::filesystem::directory_iterator(Path))
	{
		// Deduce the shader type using our naming convention
		// ... In the future I want to get rid of the naming convention
		std::string StrPath = It.path().string();
		std::transform(StrPath.begin(), StrPath.end(), StrPath.begin(), ::tolower);

		EShaderType ShaderType = EShaderType::Unknown;
		if (StrPath.find("vertex") != std::string::npos)
		{
			ShaderType = EShaderType::Vertex;
		}
		else if (StrPath.find("fragment") != std::string::npos)
		{
			ShaderType = EShaderType::Fragment;
		}
		else if (StrPath.find("geometry") != std::string::npos)
		{
			ShaderType = EShaderType::Geometry;
		}

		if (ShaderType == EShaderType::Unknown)
		{
			std::cout << "Error reading shader file: " << StrPath << "\n\tType not supported";
			continue;
		}
		// Now that we have the shader path and type
		i32 ShaderNum = LoadShader(StrPath, ShaderType);
		if (ShaderNum == -1) // Check if we were able to load the shader, if not get us out of here
		{
			return -1;
		}
		Shaders[static_cast<usize>(ShaderType)] = ShaderNum;
	}

	// If we get to this point this means all our shaders have compiled
	m_ProgramID = glCreateProgram();
	// now bind all shaders to our program
	for (int i = 0; i < NumShaders; ++i)
	{
		if (Shaders[i] != -1)
		{
			glAttachShader(m_ProgramID, Shaders[i]);
		}
	}

	glLinkProgram(m_ProgramID);

	// print linking errors if any
	i32 Success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &Success);
	if(!Success)
	{
		char InfoLog[512];
		glGetProgramInfoLog(m_ProgramID, 512, NULL, InfoLog);
		std::cout << "Shader linking failed.\n\t" << InfoLog << "\n";
		return SHADER_ERROR;
	}

	// Now that linking is done we can get rid of the shader handles
	for (int i = 0; i < NumShaders; ++i)
	{
		if (Shaders[i] != -1)
		{
			glDeleteShader(Shaders[i]);
		}
	}

	// Get uniforms
	i32 UniformCount;
	glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTES, &UniformCount);
	std::cout << "Active Attributes: " << UniformCount << "\n";

	for (i32 i = 0; i < UniformCount; i++)
	{
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetActiveAttrib(m_ProgramID, (GLuint)i, bufSize, &length, &size, &type, name);

		std::cout << "Attribute " << i << " Type: " << type << " Name: " << name << "\n";
	}
}

void CShaderProgram::Use()
{
	glUseProgram(m_ProgramID);
}

int CShaderProgram::LoadShader(std::string a_FilePath, EShaderType a_ShaderType)
{
	// Read shader file contents
	std::ifstream File;
	File.open(a_FilePath.c_str());

	if (!File.is_open())
	{
		std::cout << "Unable to open shader file: " << a_FilePath << "\n";
		return -1;
	}

	std::stringstream Buffer;
	Buffer << File.rdbuf();

	std::string ShaderString = Buffer.str();
	const char* ShaderCString = ShaderString.c_str();

	u32 GLShaderType;
	switch(a_ShaderType)
	{
		case EShaderType::Vertex:
			GLShaderType = GL_VERTEX_SHADER;
			break;
		case EShaderType::Fragment:
			GLShaderType = GL_FRAGMENT_SHADER;
			break;
		case EShaderType::Geometry:
			GLShaderType = GL_GEOMETRY_SHADER;
			break;
	}
	i32 Shader = glCreateShader(GLShaderType);
	glShaderSource(Shader, 1, &ShaderCString, NULL);
	glCompileShader(Shader);

	i32 Success;
	char InfoLog[512];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
		std::cout << "ERROR Shader compilation failed:\n\t" << InfoLog << "\n";
		return -1;
	}

	return Shader;
}

}}}