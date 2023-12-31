#include "stdafx.h"
#include "Shaders.h"
#include "Vertex.h"



Shader::~Shader()
{
	glDeleteProgram(_programId);
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);
}

int Shader::Init(char* fileVertexShader, char* fileFragmentShader)
{
	_vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if (_vertexShader == 0)
		return -1;

	_fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if (_fragmentShader == 0)
	{
		glDeleteShader(_vertexShader);
		return -2;
	}

	_programId = esLoadProgram(_vertexShader, _fragmentShader);


	//finding location of attributes
	positionAttribute = glGetAttribLocation(_programId, "a_posL");
	colorAttribute = glGetAttribLocation(_programId, "a_colL");
	uvAttribute = glGetAttribLocation(_programId, "a_texCoord");
	uv2Attribute = glGetAttribLocation(_programId, "a_texCoord2");
	normAttribute = glGetAttribLocation(_programId, "a_norm");
	binormAttribute = glGetAttribLocation(_programId, "a_binorm");
	tgtAttribute = glGetAttribLocation(_programId, "a_tgt");

	//finding location of uniforms
	textureUniform = glGetUniformLocation(_programId, "u_Texture");
	mvpUniform = glGetUniformLocation(_programId, "u_mvp");
	rotateUniform = glGetUniformLocation(_programId, "u_rotate");

	return 0;
}
void Shader::Load(ShaderResource* shaderResource) {

	_shaderResource = shaderResource;
	Init((char*)_shaderResource->VertexFilePath().c_str(), (char*)_shaderResource->FragmentFilePath().c_str());
}

void Shader::Bind() {

	glUseProgram(_programId);
}
void Shader::Unbind() {

	glUseProgram(0);
}


void Shader::SetAttributes() {

	if (positionAttribute != -1)
	{
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	}
	if (colorAttribute != -1) {

		glEnableVertexAttribArray(colorAttribute);
		glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	}
	if (uvAttribute != -1) {

		glEnableVertexAttribArray(uvAttribute);
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
	}
	if (uv2Attribute != -1) {

		glEnableVertexAttribArray(uv2Attribute);
		glVertexAttribPointer(uv2Attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv2)));
	}
	if (normAttribute != -1) {

		glEnableVertexAttribArray(normAttribute);
		glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, norm)));
	}
	if (binormAttribute != -1) {

		glEnableVertexAttribArray(binormAttribute);
		glVertexAttribPointer(binormAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, binorm)));
	}
	if (tgtAttribute != -1) {

		glEnableVertexAttribArray(tgtAttribute);
		glVertexAttribPointer(tgtAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tgt)));
	}
}

int Shader::GetUniformLocation(const std::string& name) {

	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(_programId, name.c_str());
	if (location == -1) {

		std::string generalName = _shaderResource->FragmentShaderFilename.substr(0, _shaderResource->FragmentShaderFilename.find("FS"));
		std::cout << "Warning: uniform '" << name << "' doesn't exist on shader " << generalName << "!" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SetUniform1i(const std::string& name, int value) {

	glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform1f(const std::string& name, float value) {

	glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetUniform2f(const std::string& name, float v0, float v1) {

	glUniform2f(GetUniformLocation(name), v0, v1);
}
void Shader::SetUniform2f(const std::string& name, Vector2 v) {

	glUniform2f(GetUniformLocation(name), v.x, v.y);
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {

	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}
void Shader::SetUniform3f(const std::string& name, Vector3 v) {

	glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {

	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::SetUniformMatrix4fv(const std::string& name, const Matrix& matrix) {

	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix.m[0][0]);
}


