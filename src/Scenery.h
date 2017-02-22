#pragma once

#include "BaseModel.h"
#include "Model.h"

#include <vector>
#include <deque>
#include <string>
#include <random>

// Generiert Unendliche Scene aus vordefinieren Abschnitten
class Scenery : public BaseModel
{
public:
	Scenery(float speed, int renderLimit);
	virtual ~Scenery();

	void reset();
	void loadModel(std::string Filepath, int gridLength);
	virtual void draw(const BaseCamera& Cam);
	void update(float time);
protected:
	void cleanSectorQue(float offsetToRemove);
	void fillSectorQue(float offsetToFill);

	float speedPerS;
	int renderLimit;
	// verstrichene Zeit
	float progress;
	// Abstand zurückgelegter weg -> erster Sektor im Queue
	float sectorQueOffset;
	// Summe der Sektor Längen im Queue
	int sectorQueLength;
	// Für random Zahlen
	std::default_random_engine gen;

	// Ein Abschnitt
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
