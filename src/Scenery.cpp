
#include "Scenery.h"
#include "Matrix.h"

#include <exception>

Scenery::Scenery(float speed, int renderLimit) :
	speedPerS(speed),
	renderLimit(renderLimit)
{
	reset();
}

Scenery::~Scenery()
{
	for (auto s : sectors) {
		delete s->model;
		delete s;
	}
}

void Scenery::reset()
{
	progress = 0;
	sectorQueOffset = 0;
	sectorQueLength = 0;
	sectorQue.clear();
}

void Scenery::loadModel(std::string Filepath, int gridLength)
{
	auto s = new Landscape(gridLength);
	s->model = new Model(Filepath);
	s->model->shader(this->shader());
	sectors.push_back(s);
}

void Scenery::draw(const BaseCamera & Cam)
{
	float overflowStart = progress * speedPerS - sectorQueOffset;
	int currentSectorOffset = 0;

	// Alle Sektoren rendern
	auto it = sectorQue.begin();
	for (auto s : sectorQue)
	{
		auto model = s->model;
		// z-Position relativ zum Track
		float zPos = currentSectorOffset - overflowStart;

		Matrix matPos;
		matPos.translation(0, 0, -zPos);
		model->transform(matPos * transform());

		model->draw(Cam);

		currentSectorOffset += s->length;
	}
}

void Scenery::update(float time)
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

void Scenery::cleanSectorQue(float offsetToRemove)
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

void Scenery::fillSectorQue(float offsetToFill)
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
