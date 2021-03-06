//
//  Texture.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 24.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Texture.h"
#include "FreeImage.h"

#include <assert.h>
#include <exception>
#include <iostream>
#include <algorithm>


Texture* Texture::pDefaultTex = NULL;
Texture* Texture::pDefaultEmitTex = NULL;
Texture::SharedTexMap Texture::SharedTextures;

Texture* Texture::defaultTex()
{
	if (pDefaultTex)
		return pDefaultTex;

	unsigned char* data = new unsigned char[4 * 4 * 4];
	assert(data);
	std::memset(data, 255, 4 * 4 * 4);
	pDefaultTex = new Texture(4, 4, data);
	delete[] data;

	return pDefaultTex;
}

Texture * Texture::defaultEmitTex()
{
	if (pDefaultEmitTex)
		return pDefaultEmitTex;

	unsigned char* data = new unsigned char[4 * 4 * 4];
	assert(data);
	std::memset(data, 255, 4 * 4 * 4);
	pDefaultEmitTex = new Texture(4, 4, data);
	delete[] data;

	return pDefaultEmitTex;
}

const Texture* Texture::LoadShared(const char* Filename, bool SRGB)
{
	std::string path = Filename;

	std::transform(path.begin(), path.end(), path.begin(), ::tolower);

	SharedTexMap::iterator it = SharedTextures.find(path);

	if (it != SharedTextures.end())
	{
		it->second.RefCount++;
		return it->second.pTex;
	}

	Texture* pTex = new Texture();
	if (!pTex->load(Filename, SRGB))
	{
		delete pTex;
		std::cout << "WARNING: Texture " << Filename << " not loaded (not found).\n";
		return NULL;
	}

	TexEntry TE;
	TE.pTex = pTex;
	TE.RefCount = 1;
	SharedTextures[path] = TE;

	return pTex;
}

void Texture::ReleaseShared(const Texture* pTex)
{
	for (SharedTexMap::iterator it = SharedTextures.begin(); it != SharedTextures.end(); ++it)
	{
		if (it->second.pTex == pTex)
		{
			it->second.RefCount--;
			if (it->second.RefCount <= 0)
			{
				delete it->second.pTex;
				SharedTextures.erase(it);
			}
			return;
		}
	}
}

Texture::Texture() : m_TextureID(0), CurrentTextureUnit(0), Width(0), Height(0) {}

Texture::Texture(unsigned int width, unsigned int height, unsigned char* data) : m_TextureID(0), CurrentTextureUnit(0), Width(0), Height(0)
{
	bool Result = create(width, height, data);
	if (!Result) throw std::exception();
}

Texture::Texture(const char* Filename, bool SRGB) : m_TextureID(0), CurrentTextureUnit(0), Width(0), Height(0)
{
	bool Result = load(Filename, SRGB);
	if (!Result) throw std::exception();
}

Texture::~Texture()
{
	release();
}

void Texture::release()
{
	if (isValid())
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = -1;
	}
}

bool Texture::isValid() const
{
	return m_TextureID > 0;
}

unsigned int Texture::width() const
{
	return Width;
}
unsigned int Texture::height() const
{
	return Height;
}

GLuint Texture::ID() const
{
	return m_TextureID;
}

bool Texture::load(const char* Filename, bool SRGB)
{
	release();
	FREE_IMAGE_FORMAT ImageFormat = FreeImage_GetFileType(Filename, 0);
	if (ImageFormat == FIF_UNKNOWN)
		ImageFormat = FreeImage_GetFIFFromFilename(Filename);

	if (ImageFormat == FIF_UNKNOWN)
	{
		std::cout << "Warning: Unkown texture format: " << Filename << std::endl;
		return false;
	}

	FIBITMAP* pBitmap = FreeImage_Load(ImageFormat, Filename);

	if (pBitmap == NULL)
	{
		std::cout << "Warning: Unable to open texture image " << Filename << std::endl;
		return false;
	}

	FREE_IMAGE_TYPE Type = FreeImage_GetImageType(pBitmap);
	assert(Type == FIT_BITMAP);

	Width = FreeImage_GetWidth(pBitmap);
	Height = FreeImage_GetHeight(pBitmap);
	unsigned int bpp = FreeImage_GetBPP(pBitmap);
	assert(bpp == 32 || bpp == 16 || bpp == 24);

	unsigned char* data = new unsigned char[Width * Height * 4];
	unsigned char* dataPtr = data - 1;

	if (data == NULL)
	{
		FreeImage_Unload(pBitmap);
		return false;
	}


	RGBQUAD c;
	for (unsigned int i = 0; i < Height; ++i)
		for (unsigned int j = 0; j < Width; ++j)
		{
			FreeImage_GetPixelColor(pBitmap, j, Height - i - 1, &c);
			*(++dataPtr) = c.rgbRed;
			*(++dataPtr) = c.rgbGreen;
			*(++dataPtr) = c.rgbBlue;
			if (bpp == 32)
				*(++dataPtr) = c.rgbReserved;
			else
				*(++dataPtr) = 255;
		}

	FreeImage_Unload(pBitmap);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, SRGB ? GL_SRGB_ALPHA : GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Maximale anisotorpen-filterung setzen
	if (GLEW_EXT_texture_filter_anisotropic) {
		float aniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] data;
	return true;
}

bool Texture::create(unsigned int width, unsigned int height, unsigned char* data)
{
	release();

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	Width = width;
	Height = height;

	return true;
}

void Texture::clampToEdge() const
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activate(int slot) const
{
	if (m_TextureID == 0 || slot < 0 || slot > 7)
		return;

	CurrentTextureUnit = slot;

	glActiveTexture(GL_TEXTURE0 + CurrentTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::deactivate() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	if (CurrentTextureUnit > 0)
		glActiveTexture(GL_TEXTURE0 + CurrentTextureUnit - 1);
	CurrentTextureUnit = 0;

}
