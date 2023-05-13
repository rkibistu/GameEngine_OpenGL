#pragma once

#include <unordered_map>

#include "Utilities/utilities.h"

struct ShaderResource {

	unsigned int ID;

	std::string Path;
	std::string VertexShaderFilename;
	std::string FragmentShaderFilename;

	std::string VertexFilePath() { return Path + VertexShaderFilename; }
	std::string FragmentFilePath() { return Path + FragmentShaderFilename; }
};

class Shader 
{
public:
	~Shader();

	int Init(char* fileVertexShader,char* fileFragmentShader);

	void Load(ShaderResource* shaderResource);

	void Bind();
	void Unbind();

	// Set all attributes that exist in Vertex class, using vertex buffers that are loaded 
	void SetAttributes();

	//Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string& name, const Matrix& matrix);

private:
	int GetUniformLocation(const std::string& name);

private:
	ShaderResource* _shaderResource;
	GLint _programId, _vertexShader, _fragmentShader;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	// Attributes of vertex class
	GLint positionAttribute;
	GLint colorAttribute;
	GLint uvAttribute;
	GLint uv2Attribute;
	GLint normAttribute;
	GLint binormAttribute;
	GLint tgtAttribute;

	// Uniforms
	GLint textureUniform;
	GLint mvpUniform;
	GLint rotateUniform;
};