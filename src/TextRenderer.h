#pragma once

#include "Shaders.h"
#include "Model.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include "Utilities/utilities.h"


class TextRenderer {

public:
	struct Character {

		unsigned int TextureID;
		Vector2 Size;
		Vector2 Bearing;
		unsigned int Advance;
	};

public:
	
	int Init();

	void RenderText();
	void ReallyRenderText(std::string text, float x, float y, float scale, Vector3 color);

private:
	void SetUniforms();

private:
	std::map<char, Character> Characters;

	Shader* _shader;
	Model* _model;
};