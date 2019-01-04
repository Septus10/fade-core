#pragma once

#include <string>
#include <unordered_map>
#include <Core/Containers/Array.hpp>
#include <Core/definitions.hpp>

#include <glm.hpp>

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

	i32 GetID() { return m_ProgramID; }

	//==================================================
	// Setting uniform values
	//==================================================
		// Scalars
	void SetBoolValue	(std::string a_Name, b32 a_Value);
	void SetIntValue	(std::string a_Name, i32 a_Value);
	void SetUintValue	(std::string a_Name, u32 a_Value);
	void SetFloatValue	(std::string a_Name, f32 a_Value);
	void SetDoubleValue	(std::string a_Name, f64 a_Value);
		// Vectors
			// Bool
	void SetVec(std::string a_Name, glm::bvec2 a_Vec);
	void SetVec(std::string a_Name, glm::bvec3 a_Vec);
	void SetVec(std::string a_Name, glm::bvec4 a_Vec);
			// Signed integer
	void SetVec(std::string a_Name, glm::ivec2 a_Vec);
	void SetVec(std::string a_Name, glm::ivec3 a_Vec);
	void SetVec(std::string a_Name, glm::ivec4 a_Vec);
			// Unsigned integer
	void SetVec(std::string a_Name, glm::uvec2 a_Vec);
	void SetVec(std::string a_Name, glm::uvec3 a_Vec);
	void SetVec(std::string a_Name, glm::uvec4 a_Vec);
			// Float
	void SetVec(std::string a_Name, glm::fvec2 a_Vec);
	void SetVec(std::string a_Name, glm::fvec3 a_Vec);
	void SetVec(std::string a_Name, glm::fvec4 a_Vec);
			// Double
	void SetVec(std::string a_Name, glm::dvec2 a_Vec);
	void SetVec(std::string a_Name, glm::dvec3 a_Vec);
	void SetVec(std::string a_Name, glm::dvec4 a_Vec);
		// Matrices
			// Same proportions
	void SetMat(std::string a_Name, glm::mat2 a_Mat);
	void SetMat(std::string a_Name, glm::mat3 a_Mat);
	void SetMat(std::string a_Name, glm::mat4 a_Mat);
private:
	bool GetUniformLocation(std::string a_Name, i32& oa_Location);
	i32 LoadShader(std::string a_FileContents, EShaderType a_ShaderType);

	std::unordered_map<std::string, i32> m_UniformLocations;

	u32 m_ProgramID;

};

}}}