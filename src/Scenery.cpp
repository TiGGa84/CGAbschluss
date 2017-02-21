
#include "Scenery.h"
#include "Matrix.h"

#include <exception>

Scenery::Scenery(double speed, int renderLimit) :
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
	double overflowStart = progress * speedPerS - sectorQueOffset;
	int currentSectorOffset = 0;

	// Alle Sektoren rendern
	auto it = sectorQue.begin();
	while (it != sectorQue.end())
	{
		auto s = (*it++);
		auto model = s->model;
		// z-Position relativ zum Track
		double zPos = currentSectorOffset - overflowStart;

		Matrix matPos;
		matPos.translation(0, 0, (float)-zPos);
		model->transform(matPos * transform());

		model->draw(Cam);

		currentSectorOffset += s->length;
	}
}

void Scenery::update(double time, double frametime)
{
	progress = time;
	// Zurückgelegter weg
	double dist = progress * speedPerS;
	// Check queue end
	double overflowStart = dist - sectorQueOffset;
	double overflowEnd = sectorQueLength - overflowStart - renderLimit;
	fillSectorQue(overflowEnd);
	// Check queue start
	cleanSectorQue(overflowStart);

}

void Scenery::cleanSectorQue(double offsetToRemove)
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

void Scenery::fillSectorQue(double offsetToFill)
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
