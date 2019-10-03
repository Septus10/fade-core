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

	FW::FileWatcher::Get().addWatch(a_FolderPath, this);

	const usize NumShaders = static_cast<usize>(EShaderType::NUM_SHADERTYPES);
	m_Shaders.resize(NumShaders);
	std::fill(m_Shaders.begin(), m_Shaders.end(), -1);
	// Go through all the files in the directory
	for (auto& It: std::filesystem::directory_iterator(Path))
	{
		// Deduce the shader type using our naming convention
		// ... In the future I want to get rid of the naming convention
		std::string StrPath = It.path().string();
		std::transform(StrPath.begin(), StrPath.end(), StrPath.begin(), ::tolower);

		EShaderType ShaderType = GetShaderType(StrPath);
		u32 GLShaderType;
		switch (ShaderType)
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


		if (ShaderType == EShaderType::Unknown)
		{
			std::cout << "Error reading shader file: " << StrPath << "\n\tType not supported";
			continue;
		}
		// Now that we have the shader path and type
		usize shaderTypeIndex = static_cast<usize>(ShaderType);
		m_Shaders[shaderTypeIndex] = glCreateShader(GLShaderType);
		i32 loadResult = LoadShader(StrPath, ShaderType, m_Shaders[shaderTypeIndex]);
		if (loadResult == -1) // Check if we were able to load the shader, if not get us out of here
		{
			return -1;
		}
	}

	// If we get to this point this means all our shaders have compiled
	m_ProgramID = glCreateProgram();
	// now bind all shaders to our program
	for (int i = 0; i < NumShaders; ++i)
	{
		if (m_Shaders[i] != -1)
		{
			glAttachShader(m_ProgramID, m_Shaders[i]);
		}
	}

	LinkProgram();

	return true;
}

void CShaderProgram::Use()
{
	glUseProgram(m_ProgramID);
}

void CShaderProgram::SetBoolValue(std::string a_Name, b32 a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1i(Location, a_Value);
	}
}

void CShaderProgram::SetIntValue(std::string a_Name, i32 a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1i(Location, a_Value);
	}
}

void CShaderProgram::SetUintValue(std::string a_Name, u32 a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1ui(Location, a_Value);
	}
}

void CShaderProgram::SetFloatValue(std::string a_Name, f32 a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1f(Location, a_Value);
	}
}

void CShaderProgram::SetDoubleValue(std::string a_Name, f64 a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1d(Location, a_Value);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::bvec2 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform2iv(Location, 1, reinterpret_cast<const i32*>(&a_Vec[0]));
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::bvec3 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform3iv(Location, 1, reinterpret_cast<const i32*>(&a_Vec[0]));
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::bvec4 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform4iv(Location, 1, reinterpret_cast<const i32*>(&a_Vec[0]));
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::ivec2 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform2iv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::ivec3 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform3iv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::ivec4 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform4iv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::uvec2 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform2uiv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::uvec3 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform3uiv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::uvec4 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform4uiv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::fvec2 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform2fv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::fvec3 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform3fv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::fvec4 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform4fv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::dvec2 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform2dv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::dvec3 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform3dv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, glm::dvec4 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform4dv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetMat(std::string a_Name, glm::mat2 a_Mat)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniformMatrix2fv(Location, 1, GL_FALSE, &a_Mat[0][0]);
	}
}

void CShaderProgram::SetMat(std::string a_Name, glm::mat3 a_Mat)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniformMatrix3fv(Location, 1, GL_FALSE, &a_Mat[0][0]);
	}
}

void CShaderProgram::SetMat(std::string a_Name, glm::mat4 a_Mat)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniformMatrix4fv(Location, 1, GL_FALSE, &a_Mat[0][0]);
	}
}

i32 CShaderProgram::GetUniformLocation(std::string a_Name)
{
	i32 OutUniformLocation;
	GetUniformLocation(a_Name, OutUniformLocation);
	return OutUniformLocation;
}

void CShaderProgram::handleFileAction(FW::WatchID a_WatchID, const FW::String & a_Dir, const FW::String & a_Filename, FW::Action a_Action)
{
	EShaderType shaderType = GetShaderType(a_Filename);
	// first delete all the shaders
	const usize shaderTypeIndex = static_cast<const usize>(shaderType);

	std::string completePath = a_Dir + a_Filename;

	i32 loadResult = LoadShader(completePath, shaderType, m_Shaders[shaderTypeIndex]);

	if (loadResult == -1)
	{
		return;
	}

	LinkProgram();
}

bool CShaderProgram::GetUniformLocation(std::string a_Name, i32 & oa_Location)
{
	if (m_UniformLocations.find(a_Name) != m_UniformLocations.end())
	{
		oa_Location = m_UniformLocations[a_Name];
		return true;
	}
	oa_Location = -1;
	return false;
}

int CShaderProgram::LoadShader(std::string a_FilePath, EShaderType a_ShaderType, i32 a_Shader)
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

	glShaderSource(a_Shader, 1, &ShaderCString, NULL);
	glCompileShader(a_Shader);

	i32 Success;
	char InfoLog[512];
	glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(a_Shader, 512, NULL, InfoLog);
		std::cout << "ERROR Shader compilation failed:\n\t" << InfoLog << "\n";
		return -1;
	}

	return a_Shader;
}

i32 CShaderProgram::LinkProgram()
{
	glLinkProgram(m_ProgramID);

	// print linking errors if any
	i32 Success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		char InfoLog[512];
		glGetProgramInfoLog(m_ProgramID, 512, NULL, InfoLog);
		std::cout << "Shader linking failed.\n\t" << InfoLog << "\n";
		return SHADER_ERROR;
	}

	m_UniformLocations.clear();

	// Get uniforms
	i32 UniformCount;
	glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORMS, &UniformCount);
	std::cout << "Active Uniforms: " << UniformCount << "\n";

	GLint MaxUniformNameLength = 0;
	glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformNameLength);
	std::vector<GLchar> NameData(MaxUniformNameLength);

	for (i32 i = 0; i < UniformCount; i++)
	{
		GLint	Size;			// size of the variable
		GLenum	Type;			// type of the variable (float, vec3 or mat4, etc)
		GLsizei ActualLength;	// name length

		glGetActiveUniform(m_ProgramID, (GLuint)i, MaxUniformNameLength, &ActualLength, &Size, &Type, &NameData[0]);
		std::string Name((char*)&NameData[0], ActualLength);

		std::cout << "Attribute:\t" << i << "\nType:\t\t" << Type << "\nName:\t\t" << Name << "\n";

		i32 Location = glGetUniformLocation(m_ProgramID, Name.c_str());
		if (Location == -1)
		{
			std::cout << "\nError couldn't get uniform with name: " << Name << "\n";
			continue;
		}

		m_UniformLocations[Name] = Location;
	}

	return 0;
}

char asciitolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

EShaderType CShaderProgram::GetShaderType(std::string a_FilePath)
{
	// we need lower
	std::transform(a_FilePath.begin(), a_FilePath.end(), a_FilePath.begin(), asciitolower);
	EShaderType type = EShaderType::Unknown;
	if (a_FilePath.find("vertex") != std::string::npos)
	{
		type = EShaderType::Vertex;
	}
	else if (a_FilePath.find("fragment") != std::string::npos)
	{
		type = EShaderType::Fragment;
	}
	else if (a_FilePath.find("geometry") != std::string::npos)
	{
		type = EShaderType::Geometry;
	}
	return type;
}

}}}