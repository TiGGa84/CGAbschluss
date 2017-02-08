#include "rgbimage.h"
#include "color.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <algorithm>

RGBImage::RGBImage(unsigned int Width, unsigned int Height) {
	assert(Width > 0 && Height > 0);
	m_Width = Width;
	m_Height = Height;
	m_Image = new Color[Width * Height];
}

RGBImage::~RGBImage() {
	delete m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {
	assert(x < m_Width && y < m_Height);
	m_Image[m_Width * y + x] = c;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const {
	assert(x < m_Width && y < m_Height);
	return m_Image[m_Width * y + x];
}

unsigned int RGBImage::width() const {
	return m_Width;
}

unsigned int RGBImage::height() const {
	return m_Height;
}

unsigned char RGBImage::convertColorChannel(float v) {
	v = std::max(0.0f, std::min(1.0f, v));
	return static_cast<unsigned char>(v * 255.999f);
}

bool RGBImage::saveToDisk(const char* Filename) {

	FILE* file = fopen(Filename, "wb");
	if (file == NULL) {
		perror("Failed to open File");
		return false;
	}

	uint32_t width = m_Width;
	uint32_t height = m_Height;

	// Inhalt muss vielfaches von 4 sein
	uint32_t padding = (4 - (width * 3) % 4) % 4;
	// Größe Daten + padding
	uint32_t paddedsize = ((width * 3) + padding) * height;
	// Größe Daten + Header
	uint32_t filesize = paddedsize + 54;

	// 2 Magic-Bytes + 52 Bytes Header = 54 Byte
	uint32_t headers[13];
	// Headers
	headers[0] = filesize; // bfSize
	headers[1] = 0; // 2 bfReserved Bytes
	headers[2] = 54; // bfOffbits
	headers[3] = 40; // biSize
	headers[4] = width; // biWidth
	headers[5] = height; // biHeight
	headers[6] = (24 << 16) | (1); // biPlanes + biBitCount
	headers[7] = 0; // biCompression
	headers[8] = 0; // biSizeImage
	headers[9] = 0; // biXPelsPerMeter
	headers[10] = 0; // biYPelsPerMeter
	headers[11] = 0; // biClrUsed
	headers[12] = 0; // biClrImportant

	fprintf(file, "BM");

	fwrite(headers, sizeof(char), sizeof(headers), file);

	// Bild Daten
	// Bytes werden von unten nach oben gespeichert
	for (unsigned int y = height; y-- > 0;) {
		for (unsigned int x = 0; x <= width - 1; x++) {
			// write b,g,r
			Color c = getPixelColor(x, y);
			fputc(convertColorChannel(c.B), file);
			fputc(convertColorChannel(c.G), file);
			fputc(convertColorChannel(c.R), file);
		}
		// Padding für jede Zeile
		for (uint32_t i = 0; i < padding; i++) {
			fputc(0, file);
		}
	}

	fclose(file);

	return true;
}
