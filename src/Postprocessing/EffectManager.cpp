#include "stdafx.h"
#include "EffectManager.h"



void EffectManager::Init() {

	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &_maxRenderbufferSize);
	// check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
	if ((_maxRenderbufferSize <= _texWidth) ||
		(_maxRenderbufferSize <= _texHeight))
	{
		// cannot use framebuffer objects as we need to create
		// a depth buffer as a renderbuffer object
		// return with appropriate error
		std::cout << "CANN NOT USE FRAMEBUFFE OBJECTS!\n";
		return;
	}

	//creem obietele OpENGL
	glGenFramebuffers(2, _framebuffer);
	glGenRenderbuffers(2, _depthRenderBuffer);
	glGenTextures(2, _texture);

	//setam texturile, frame buffer (color and depth attachements)
	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, _texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texWidth, _texHeight,
			0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// bind renderbuffer and create a 16-bit depth buffer
		// width and height of renderbuffer = width and height of 
		// the texture
		glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
			_texWidth, _texHeight);

		// bind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer[i]);
		// specify texture as color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, _texture[i], 0);

		// specify depth_renderbufer as depth attachment
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, _depthRenderBuffer[i]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//creem obiectele de tip textura, tp fiecare textura
	Texture* tempTexture = CreateTextureObject(_texture[0]);
	_textures.push_back(tempTexture);
	tempTexture = CreateTextureObject(_texture[1]);
	_textures.push_back(tempTexture);

	//shader-ul
	Shader* tempShader = SceneManager::GetInstance().GetTextShader();

	//creem effectquad, acesta cva realiza deesenarea efectiva
	_effectQuad = new EffectQuad();
	_effectQuad->SetTexture(_textures[0]);
	_effectQuad->SetShader(tempShader);

}
void EffectManager::Destroy() {

	// cleanup
	glDeleteFramebuffers(2, _framebuffer);
	glDeleteRenderbuffers(2, _depthRenderBuffer);
	glDeleteTextures(2, _texture);

	for (int i = 0; i < _textures.size(); i++) {

		delete _textures[i]->GetTextureResource();
		delete _textures[i];
	}

	if (_effectQuad) {

		delete _effectQuad;
		_effectQuad = nullptr;
	}
}

void EffectManager::Draw(ESContext* esContext) {

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer[0]);
	// check for framebuffer complete
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		// render to texture using FBO
		// clear color and depth buffer
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_sceneManager.Draw(esContext);


		// render to window system provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw to a quad
		//DrawQuad();
		_effectQuad->Draw();

		eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
	}
}

Texture* EffectManager::CreateTextureObject(GLuint id) {

	TextureResource* textureResource = new TextureResource();
	textureResource->Type = TEXTURE_TYPE_2D;
	Texture* texture = new Texture();
	texture->SetTextureId(id);
	texture->SetTextureResource(textureResource);
	return texture;
}
