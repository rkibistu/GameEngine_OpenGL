#pragma once

#include <vector>
#include <string>

#include "../Utilities/utilities.h" 


#define NFG_SUCCES 1
#define NFG_ERROR 40
#define NFG_NOT_FOUND 41
#define NFG_BAD_FORMAT 42

struct Vertex;

/*
	Parseaza fisier NFG.

	Functia Load()
	RETURN	NFG_SUCCES daca parsarea a fost facuta cu succes.
			NFG_ERROR code daca a esuat. Populeaza _errorMessage cu detalii despre eroare.
				use GetError() to see the error
*/

class NfgParser
{
public:
	int Load(std::string filepath);

	inline std::string GetError() { return _errorMessage; }

private:
	int GetBufferCount(std::string line, std::string nfgElement, unsigned int& count);
	int GetVertexPosition(std::string line, Vector3& position);

	int GetVertexAttribute(std::string line, std::string nfgElement, Vector3& vector);
	int GetVertexAttribute(std::string line, std::string nfgElement, Vector2& vector);

	int SetVector(std::string str, Vector3& vector);
	int SetVector(std::string str, Vector2& vector);
private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned short> _indices;

	std::string _errorMessage;
};

class NfgElements {
public:
	static std::string StartElement;
	static std::string EndElement;
	static std::string OpeningBracket;
	static std::string ClosingBracket;
	static std::string VerticesCount;
	static std::string VerticesPosition;
	static std::string VerticesNormal;
	static std::string VerticesBinormal;
	static std::string VerticesTangent;
	static std::string VerticesUV;
	static std::string IndicesCount;
};

