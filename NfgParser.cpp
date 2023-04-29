#include "stdafx.h"
#include "NfgParser.h"
#include "Vertex.h"
#include <fstream>
#include <sstream>

// NfgElements class

std::string NfgElements::VerticesCount = "NrVertices";
std::string NfgElements::IndicesCount = "NrIndices";
std::string NfgElements::StartElement = ":";
std::string NfgElements::EndElement = ";";
std::string NfgElements::OpeningBracket = "[";
std::string NfgElements::ClosingBracket = "]";
std::string NfgElements::VerticesPosition = "pos";
std::string NfgElements::VerticesNormal = "norm";
std::string NfgElements::VerticesBinormal = "binorm";
std::string NfgElements::VerticesTangent = "tgt";
std::string NfgElements::VerticesUV = "uv";

// NfgParser class

int NfgParser::Load(std::string filepath, std::vector<Vertex>& vertices, std::vector<short>& indices) {

	int res;
	unsigned int verticesCount, indicesCount;
	short token, token1, token2, token3;
	Vertex temp;
	char c;
	std::string line;

	std::ifstream file(filepath);
	if (!file.is_open()) {

		_errorMessage = "Coudn't open file " + filepath;
		return NFG_ERROR;
	}

	std::getline(file, line);
	res = GetBufferCount(line,NfgElements::VerticesCount, verticesCount);
	if (res != NFG_SUCCES)
		return res;
	
	vertices.reserve(verticesCount);
	for (int i = 0; i < verticesCount; i++) {

		std::getline(file, line);
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
			line.end());

		res = GetVertexAttribute(line,NfgElements::VerticesPosition, temp.pos);
		if (res != NFG_SUCCES)
			return res;
		res = GetVertexAttribute(line,NfgElements::VerticesNormal, temp.norm);
		if (res != NFG_SUCCES)
			return res;
		res = GetVertexAttribute(line,NfgElements::VerticesBinormal, temp.binorm);
		if (res != NFG_SUCCES)
			return res;
		res = GetVertexAttribute(line,NfgElements::VerticesTangent, temp.tgt);
		if (res != NFG_SUCCES)
			return res;
		res = GetVertexAttribute(line,NfgElements::VerticesUV, temp.uv);
		if (res != NFG_SUCCES)
			return res;

		vertices.push_back(temp);
	}

	std::getline(file, line);
	res = GetBufferCount(line, NfgElements::IndicesCount, indicesCount);
	if (res != NFG_SUCCES)
		return res;

	indices.reserve(indicesCount);
	for (int i = 0; i < indicesCount / 3; i++) {


		file >> token >> c >> token1 >> c >> token2 >> c >> token3;
		indices.push_back(token1);
		indices.push_back(token2);
		indices.push_back(token3);
	}

	file.close();

	return NFG_SUCCES;
}

int NfgParser::GetBufferCount(std::string line, std::string nfgElement, unsigned int& count) {

	unsigned int pos = line.find(nfgElement);
	if (pos == std::string::npos) {

		_errorMessage = "Can't find " + nfgElement + " on line: '" + line + "'";
		return NFG_NOT_FOUND;
	}

	pos = line.find(NfgElements::StartElement, pos);
	if (pos == std::string::npos) {

		_errorMessage = "Missing " + NfgElements::StartElement + " on line: '" + line + "'";
		return NFG_NOT_FOUND;
	}
	std::string value = line.substr(pos + 1, line.length() - pos - 1);

	value.erase(std::remove_if(value.begin(), value.end(), ::isspace),
		value.end());

	count = std::stoi(value);
	return NFG_SUCCES;
}

int NfgParser::GetVertexAttribute(std::string line, std::string nfgElement, Vector3& vector) {

	int res;
	unsigned int posFinal;
	unsigned int pos = line.find(nfgElement);
	if (pos == std::string::npos) {

		_errorMessage = "Can't find " + nfgElement + " on line: '" + line + "'";
		return NFG_NOT_FOUND;
	}

	pos = line.find(NfgElements::StartElement, pos);
	posFinal = line.find(NfgElements::EndElement, pos);
	std::string value = line.substr(pos + 1, posFinal - pos - 1);


	res = SetVector(value, vector);
	if (res != NFG_SUCCES) {
		_errorMessage = "Error parse vector for " + nfgElement + " on line: '" + line + "'";
		return NFG_NOT_FOUND;
	}

	return NFG_SUCCES;
}
int NfgParser::GetVertexAttribute(std::string line, std::string nfgElement, Vector2& vector) {

	int res;
	unsigned int posFinal;
	unsigned int pos = line.find(nfgElement);
	if (pos == std::string::npos){

		_errorMessage = "Can't find " + nfgElement + " on line: '" + line + "'";
		return NFG_NOT_FOUND;
	}

	pos = line.find(NfgElements::StartElement, pos);
	posFinal = line.find(NfgElements::EndElement, pos);
	std::string value = line.substr(pos + 1, posFinal - pos - 1);


	res = SetVector(value, vector);
	if (res != NFG_SUCCES) {
		_errorMessage = "Error parse vector for " + nfgElement + " on line: '" + line + "'";
		return NFG_NOT_FOUND;
	}

	return NFG_SUCCES;
}

// input de forma [x,y,z] sau [x,y]
int NfgParser::SetVector(std::string str, Vector3& vector) {

	std::istringstream ss(str);

	float arr[3];
	char c;
	ss >> c; // read the opening bracket

	if (c != NfgElements::OpeningBracket[0]) 
		return NFG_NOT_FOUND;
	

	ss >> vector.x >> c; //read value and comma
	ss >> vector.y >> c;
	ss >> vector.z >> c;

	ss >> c; // read the closing bracket
	if (c != NfgElements::ClosingBracket[0])
		return NFG_BAD_FORMAT;

	return NFG_SUCCES;
}
int NfgParser::SetVector(std::string str, Vector2& vector) {

	std::istringstream ss(str);

	float arr[2];
	char c;
	ss >> c; // read the opening bracket

	if (c != NfgElements::OpeningBracket[0])
		return NFG_BAD_FORMAT;

	ss >> vector.x >> c; //read value and comma
	ss >> vector.y >> c;

	ss >> c; // read the closing bracket
	if (c != NfgElements::ClosingBracket[0])
		return NFG_BAD_FORMAT;

	return NFG_SUCCES;
}