
#include "Track.h"
#include "Matrix.h"
#include "PhongShader.h"

#include "FreeImage.h"

#include <exception>

#define ASSET_DIRECTORY "../../assets/"

Track::Track(Model* model, double speed, int renderLimit) :
	model(model),
	speedPerS(speed),
	renderLimit(renderLimit),
	progress(0),
	sectorQueOffset(0),
	sectorQueLength(0)
{

	emptySector = new Sector(5);
	sectorQueLength += emptySector->length;
	sectorQue.push_back(emptySector);

	readSector(ASSET_DIRECTORY"lane1.png");
	readSector(ASSET_DIRECTORY"lane2.png");
	readSector(ASSET_DIRECTORY"lane3.png");
	readSector(ASSET_DIRECTORY"lane4.png");
	readSector(ASSET_DIRECTORY"lane5.png");

}

Track::~Track()
{
	delete model;
	delete emptySector;
	for (Sector* s : sectors) delete s;
}

void Track::draw(const BaseCamera & Cam)
{
	double overflowStart = progress * speedPerS - sectorQueOffset;
	int currentSectorOffset = 0;

	// Alle Sektoren rendern
	std::deque<Sector*>::iterator it = sectorQue.begin();
	while (it != sectorQue.end())
	{
		Sector* s = (*it++);
		for (Obstacle& o : s->obstacles)
		{
			// z-Position relativ zum Track
			double zPos = currentSectorOffset - overflowStart + o.z;
			// Clip
			if (zPos < 0 || zPos > renderLimit) continue;
			
			Matrix matPos;
			matPos.translation((float)o.x, 0, (float)-zPos);
			model->transform(matPos * transform());

			dynamic_cast<PhongShader*>(model->shader())->emitColor(o.emitColor);
			model->draw(Cam);
			
		}

		currentSectorOffset += s->length;
	}
}

void Track::update(double dtime)
{
	progress += dtime;
	// Zurückgelegter weg
	double dist = progress * speedPerS;
	// Check queue end
	double overflowStart = dist - sectorQueOffset;
	double overflowEnd = sectorQueLength - overflowStart - renderLimit;
	fillSectorQue(overflowEnd);
	// Check queue start
	cleanSectorQue(overflowStart);
	
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

	Sector* s = new Sector(height);
	Color emit;
	RGBQUAD c;
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			FreeImage_GetPixelColor(pBitmap, j, i, &c);
			if (c.rgbReserved == 0) continue;
			emit.R = c.rgbRed / 255.0f;
			emit.G = c.rgbGreen / 255.0f;
			emit.B = c.rgbBlue / 255.0f;
			s->obstacles.push_back(Obstacle(i, j - 1, emit));
		}
	}
	FreeImage_Unload(pBitmap);

	sectors.push_back(s);
}

void Track::cleanSectorQue(double offsetToRemove)
{
	while (offsetToRemove > sectorQue.front()->length)
	{
		Sector* s = sectorQue.front();
		offsetToRemove -= s->length;
		sectorQueLength -= s->length;
		sectorQueOffset += s->length;
		sectorQue.pop_front();
	}
}

void Track::fillSectorQue(double offsetToFill)
{
	while (offsetToFill < 0)
	{
		// Random int [0, n]
		std::uniform_int_distribution<int> dist(0, sectors.size() - 1);
		Sector* s = sectors[dist(gen)];
		offsetToFill += s->length;
		sectorQueLength += s->length;
		sectorQue.push_back(s);
	}
}
