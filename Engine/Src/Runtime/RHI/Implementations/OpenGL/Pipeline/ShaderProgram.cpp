#include <RHI/Pipeline/ShaderProgram.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string> 

#include <Core/Log.hpp>

#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h> 

namespace Fade { inline namespace RHI { inline namespace Pipeline {

CShaderProgram::CShaderProgram()
	: m_ProgramID(-1)
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
		FADE_LOG(Error, RHI, "ERROR: Path provided is not a directory\n\t", a_FolderPath);
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

void CShaderProgram::Bind() const
{
	glUseProgram(m_ProgramID);
}

void CShaderProgram::Unbind() const
{
	glUseProgram(GL_NONE);
}

void CShaderProgram::SetBoolValue(std::string a_Name, bool a_Value)
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

void CShaderProgram::SetFloatValue(std::string a_Name, float a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1f(Location, a_Value);
	}
}

void CShaderProgram::SetDoubleValue(std::string a_Name, double a_Value)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform1d(Location, a_Value);
	}
}

void CShaderProgram::SetVec(std::string a_Name, Fade::Math::Vec2 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform2fv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, Fade::Math::Vec3 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform3fv(Location, 1, &a_Vec[0]);
	}
}

void CShaderProgram::SetVec(std::string a_Name, Fade::Math::Vec4 a_Vec)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniform4fv(Location, 1, &a_Vec[0]);
	}
}

/*
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
*/

void CShaderProgram::SetMat(std::string a_Name, Fade::Math::Mat2 a_Mat)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniformMatrix2fv(Location, 1, GL_FALSE, &a_Mat[0][0]);
	}
}

void CShaderProgram::SetMat(std::string a_Name, Fade::Math::Mat3 a_Mat)
{
	i32 Location;
	if (GetUniformLocation(a_Name, Location))
	{
		glUniformMatrix3fv(Location, 1, GL_FALSE, &a_Mat[0][0]);
	}
}

void CShaderProgram::SetMat(std::string a_Name, Fade::Math::Mat4 a_Mat)
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

	if (m_ProgramID == -1)
	{
		LoadShaderProgram(a_Dir);
		return;
	}

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
		FADE_LOG(Error, RHI, "Unable to open shader file: ", a_FilePath);
		return -1;
	}

	std::stringstream Buffer;
	Buffer << File.rdbuf();

	std::string ShaderString = Buffer.str();
	const char* ShaderCString = ShaderString.c_str();

	glShaderSource(a_Shader, 1, &ShaderCString, NULL);
	glCompileShader(a_Shader);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		FADE_LOG(Error, RHI, "Error compiling shader: ", glewGetErrorString(err));
		return -1;
	}

	i32 Success = GL_FALSE;
	char InfoLog[512];
	glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &Success);
	if (Success != GL_TRUE)
	{
		glGetShaderInfoLog(a_Shader, 512, NULL, InfoLog);
		FADE_LOG(Error, RHI, "ERROR Shader compilation failed:\n\t", InfoLog);
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
	if (Success != GL_TRUE)
	{
		char InfoLog[512];
		glGetProgramInfoLog(m_ProgramID, 512, NULL, InfoLog);
		FADE_LOG(Error, RHI, "Shader linking failed.\n\t", InfoLog);
		return Constants::gc_ShaderError;
	}

	m_UniformLocations.clear();

	// Get uniforms
	i32 UniformCount;
	glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORMS, &UniformCount);
	FADE_LOG(Info, RHI, "Active Uniforms: ", UniformCount);

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

		FADE_LOG(Info, RHI, "Attribute:\t", i, "\nType:\t\t", Type, "\nName:\t\t", Name);

		i32 Location = glGetUniformLocation(m_ProgramID, Name.c_str());
		if (Location == -1)
		{
			FADE_LOG(Error, RHI, "\nError couldn't get uniform with name: ", Name);
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

} } }