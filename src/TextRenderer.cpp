#include "stdafx.h"
#include "TextRenderer.h"

#include "SceneManager.h"
#include "Vertex.h"

int TextRenderer::Init() {

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "Resources/Fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    SceneManager& sceneManager = SceneManager::GetInstance();

    _shader = sceneManager.GetTextShader();
    _model = new Model();
    _model->LoadQuad();
}

void TextRenderer::RenderText() {

    //return;

    if (_model == nullptr)
        return;
    if (_shader == nullptr)
        return;

    _model->BindFilled(); 
    _shader->Bind();

    _shader->SetAttributes();

    Matrix ortho;
    ortho.SetOrthographic(-100.0f, 100.0f, -100.0f, 100.0f, 0.2f, 10000.0f);

    Vector3 _position(0.0f, 0.0f, 0.0f);
    Vector3 _rotation(0.0f, 0.0f, 0.0f);
    Vector3 _scale(1.0f, 1.0f, 1.0f);

    Matrix positionMat;
    positionMat.SetTranslation(_position);

    Matrix rotationMatOY;
    rotationMatOY.SetRotationY(_rotation.y);
    Matrix rotationMatOX;
    rotationMatOX.SetRotationX(_rotation.x);
    Matrix rotationMatOZ;
    rotationMatOZ.SetRotationZ(_rotation.z);

    Matrix scaleMat;
    scaleMat.SetScale(_scale);

    Matrix model =  scaleMat * rotationMatOX * rotationMatOZ * rotationMatOY * positionMat;


    SceneManager& sceneManager = SceneManager::GetInstance();
    Camera* camera = sceneManager.GetActiveCamera();
    Matrix view = camera->GetViewMatrix();

    //setuniforms
    _shader->SetUniformMatrix4fv("u_mvp", view * ortho);


    glDrawElements(GL_TRIANGLES, _model->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

    _model->Unbind();
}

void TextRenderer::SetUniforms() {

    Matrix ortho;
    ortho.SetOrthographic(-100.0f, 100.0f, -100.0f, 100.0f, 0.2f, 10000.0f);

    Vector3 _position(0.0f, 0.0f, 0.0f);
    Vector3 _rotation(0.0f, 0.0f, 0.0f);
    Vector3 _scale(1.0f, 1.0f, 1.0f);

    Matrix positionMat;
    positionMat.SetTranslation(_position);

    Matrix rotationMatOY;
    rotationMatOY.SetRotationY(_rotation.y);
    Matrix rotationMatOX;
    rotationMatOX.SetRotationX(_rotation.x);
    Matrix rotationMatOZ;
    rotationMatOZ.SetRotationZ(_rotation.z);

 

    Matrix scaleMat;
    scaleMat.SetScale(_scale);

    Matrix model = scaleMat * rotationMatOX * rotationMatOZ * rotationMatOY * positionMat;


    SceneManager& sceneManager = SceneManager::GetInstance();
    Camera* camera = sceneManager.GetActiveCamera();
    Matrix view = camera->GetViewMatrix();

    //setuniforms
    _shader->SetUniformMatrix4fv("u_mvp", view * ortho);
}

void TextRenderer::ReallyRenderText(std::string text, float x, float y, float scale, Vector3 color) {

    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        error = glGetError();
    }

    _model->BindFilled();
    _shader->Bind();

    _shader->SetAttributes();

    SetUniforms();

    glActiveTexture(GL_TEXTURE0);
    _model->BindFilled();

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        


        while (error != GL_NO_ERROR) {
            error = glGetError();
        }

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        while (error != GL_NO_ERROR) {
            error = glGetError();
        }

        // update content of VBO memory
        _model->BindFilled();

        while (error != GL_NO_ERROR) {
            error = glGetError();
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        while (error != GL_NO_ERROR) {
            error = glGetError();
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        while (error != GL_NO_ERROR) {
            error = glGetError();
        }
        // render quad
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, _model->GetIndicesFilledCount(), GL_UNSIGNED_SHORT, nullptr);

        while (error != GL_NO_ERROR) {
            error = glGetError();
        }
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    //glBindVertexArray(0);
    _model->BindFilled();
    glBindTexture(GL_TEXTURE_2D, 0);
}