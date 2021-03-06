
#include "Track.h"
#include "Matrix.h"

#include "FreeImage.h"

#include <exception>

#define ASSET_DIRECTORY "../../assets/"

Track::Track(float speed, int renderLimit) :
	speedPerS(speed),
	renderLimit(renderLimit),
	Box(-0.5f, 0.0f, -0.5f, 0.5f, 1.0f, 0.5f)
{
	emptySector = new Sector(20);
	readSector(ASSET_DIRECTORY"lane1.png");
	readSector(ASSET_DIRECTORY"lane2.png");
	readSector(ASSET_DIRECTORY"lane3.png");
	readSector(ASSET_DIRECTORY"lane4.png");
	readSector(ASSET_DIRECTORY"lane5.png");
	readSector(ASSET_DIRECTORY"lane6.png");
	readSector(ASSET_DIRECTORY"lane7.png");
	reset();
}

Track::~Track()
{
	delete model;
	delete emptySector;
	for (auto s : sectors) delete s;
}

void Track::reset()
{
	progress = 0;
	sectorQueOffset = 0;
	sectorQueLength = emptySector->length;
	sectorQue.clear();
	sectorQue.push_back(emptySector);
}

void Track::loadModel(std::string Filepath)
{
	model = new Model(Filepath);
	model->shader(this->shader());
}

void Track::draw(const BaseCamera & Cam)
{
	float overflowStart = progress * speedPerS - sectorQueOffset;
	int currentSectorOffset = 0;

	// Alle Sektoren rendern
	for (auto s : sectorQue)
	{
		for (Obstacle& o : s->obstacles)
		{
			// z-Position relativ zum Track
			float zPos = currentSectorOffset - overflowStart + o.z;
			// Clip
			if (zPos < 0 || zPos > renderLimit) continue;

			Matrix matPos;
			matPos.translation((float)o.x, 0, -zPos);
			model->transform(matPos * transform());

			model->overrideEmit(o.emitColor);

			model->draw(Cam);
		}

		currentSectorOffset += s->length;
	}
}

void Track::update(float time)
{
	progress = time;
	// Zurückgelegter weg
	float dist = progress * speedPerS;
	// Check queue end
	float overflowStart = dist - sectorQueOffset;
	float overflowEnd = sectorQueLength - overflowStart - renderLimit;
	fillSectorQue(overflowEnd);
	// Check queue start
	cleanSectorQue(overflowStart);
	
}

// Testet eine AABB gegen alle Hindernisse
bool Track::testIntersesction(const AABB& testBox)
{
	float overflowStart = progress * speedPerS - sectorQueOffset;
	int currentSectorOffset = 0;

	for (auto s : sectorQue)
	{
		for (Obstacle& o : s->obstacles)
		{
			float zPos = currentSectorOffset - overflowStart + o.z;

			Matrix matPos;
			matPos.translation((float)o.x, 0, -zPos);
			matPos *= transform();
			if (testBox.intersects(Box * matPos)) return true;
		}

		currentSectorOffset += s->length;
	}
	return false;
}

// Bild auslesen und abschnitt erstellen
void Track::readSector(std::string filepath)
{
	FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filepath.c_str());
	if (imageFormat == FIF_UNKNOWN)	imageFormat = FreeImage_GetFIFFromFilename(filepath.c_str());
	if (imageFormat == FIF_UNKNOWN)	throw std::exception();

	FIBITMAP* pBitmap = FreeImage_Load(imageFormat, filepath.c_str());
	if (pBitmap == NULL) throw std::exception();

	if (FreeImage_GetImageType(pBitmap) != FIT_BITMAP) throw std::exception();
	if (FreeImage_GetBPP(pBitmap) != 32) throw std::exception();

	unsigned int width = FreeImage_GetWidth(pBitmap);
	if (width != 3) throw std::exception();
	unsigned int height = FreeImage_GetHeight(pBitmap);

	auto s = new Sector(height);
	Color emit;
	RGBQUAD c;
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			FreeImage_GetPixelColor(pBitmap, j, i, &c);
			// Transparent -> leer
			if (c.rgbReserved == 0) continue;
			// Farbe speichern
			emit.R = c.rgbRed / 255.0f;
			emit.G = c.rgbGreen / 255.0f;
			emit.B = c.rgbBlue / 255.0f;
			// Position aus Bild Koordinaten
			s->obstacles.push_back(Obstacle(i, j - 1, emit));
		}
	}
	FreeImage_Unload(pBitmap);

	sectors.push_back(s);
}

void Track::cleanSectorQue(float offsetToRemove)
{
	while (offsetToRemove > sectorQue.front()->length)
	{
		auto s = sectorQue.front();
		offsetToRemove -= s->length;
		sectorQueLength -= s->length;
		sectorQueOffset += s->length;
		sectorQue.pop_front();
	}
}

void Track::fillSectorQue(float offsetToFill)
{
	while (offsetToFill < 0)
	{
		// Random int [0, n]
		std::uniform_int_distribution<int> dist(0, sectors.size() - 1);
		auto s = sectors[dist(gen)];
		offsetToFill += s->length;
		sectorQueLength += s->length;
		sectorQue.push_back(s);
	}
}
