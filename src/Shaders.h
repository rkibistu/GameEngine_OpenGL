#pragma once

#include <unordered_map>

#include "Utilities/utilities.h"

struct ShaderResources {

	std::string VertexShaderPath;
	std::string FragmentShaderPath;
};

class Shader 
{
public:
	~Shader();

	int Init(char* fileVertexShader,char* fileFragmentShader);

	void Bind();
	void Unbind();

	// Set all attributes that exist in Vertex class, using vertex buffers that are loaded 
	void SetAttributes();

	//Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string& name, const Matrix& matrix);

private:
	int GetUniformLocation(const std::string& name);

private:
	GLint _programId, _vertexShader, _fragmentShader;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	// Attributes of vertex class
	GLint positionAttribute;
	GLint colorAttribute;
	GLint uvAttribute;
	GLint normAttribute;
	GLint binormAttribute;
	GLint tgtAttribute;

	// Uniforms
	GLint textureUniform;
	GLint mvpUniform;
	GLint rotateUniform;
};