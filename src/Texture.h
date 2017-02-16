//
//  Texture.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 24.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Texture__
#define __RealtimeRending__Texture__

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <string>
#include <map>

class Texture
{
public:
	Texture();
	Texture(unsigned int width, unsigned int height, unsigned char* data);
	Texture(const char* Filename, bool SRGB = true);
	~Texture();
	bool load(const char* Filename, bool SRGB = true);
	bool create(unsigned int width, unsigned int height, unsigned char* data);
	void activate(int slot = 0) const;
	void deactivate() const;
	bool isValid() const;
	unsigned int width() const;
	unsigned int height() const;
	GLuint ID() const;
	static Texture* defaultTex();
	static Texture* defaultEmitTex();
	static const Texture* LoadShared(const char* Filename, bool SRGB = true);
	static void ReleaseShared(const Texture* pTex);

protected:
	void release();
	GLuint m_TextureID;
	unsigned int Width;
	unsigned int Height;
	mutable int CurrentTextureUnit;
	static Texture* pDefaultTex;
	static Texture* pDefaultEmitTex;

	struct TexEntry
	{
		int RefCount;
		const Texture* pTex;
	};
	typedef std::map<std::string, TexEntry> SharedTexMap;
	static SharedTexMap SharedTextures;

};

#endif /* defined(__RealtimeRending__Texture__) */
