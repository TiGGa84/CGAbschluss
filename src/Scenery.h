#pragma once

#include "BaseModel.h"
#include "Model.h"

#include <vector>
#include <deque>
#include <string>
#include <random>

class Scenery : public BaseModel
{
public:
	Scenery(double speed, int renderLimit);
	virtual ~Scenery();

	void loadModel(std::string Filepath, int gridLength);
	virtual void draw(const BaseCamera& Cam);
	void update(double dtime);
protected:
	void cleanSectorQue(double offsetToRemove);
	void fillSectorQue(double offsetToFill);

	double speedPerS;
	int renderLimit;
	// verstrichene Zeit
	double progress;
	// Abstand zurückgelegter weg -> erster Sektor im Queue
	double sectorQueOffset;
	// Summe der Sektor Längen im Queue
	int sectorQueLength;
	// Für random Zahlen
	std::default_random_engine gen;

	struct Landscape
	{
		Landscape(int len) : length(len) {}
		int length;
		Model* model;
	};

	// Alle geladenen Sektoren
	std::vector<Landscape*> sectors;
	// Aktive Sektoren
	std::deque<Landscape*> sectorQue;
};
