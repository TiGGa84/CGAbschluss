#pragma once

#include "BaseModel.h"
#include "Model.h"
#include "Aabb.h"

#include <vector>
#include <deque>
#include <string>
#include <random>

// Generiert unendlich langen Weg aus Hindernissen
class Track : public BaseModel
{
public:
	Track(float speed, int renderLimit);
	virtual ~Track();

	void reset();
	void loadModel(std::string Filepath);
	virtual void draw(const BaseCamera& Cam);
	void update(float time);
	bool testIntersesction(const AABB& testBox);
protected:
	void readSector(std::string Filepath);
	void cleanSectorQue(float offsetToRemove);
	void fillSectorQue(float offsetToFill);

	// Modell für alle Hindernisse
	Model* model;
	// Kollisions-Box
	AABB Box;
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

	// Ein Hinderniss
	struct Obstacle
	{
		Obstacle(int zPos, int xPos, Color& emitColor) :
			z(zPos), x(xPos), emitColor(emitColor) {}
		int z;
		int x;
		Color emitColor;
	};

	// Ein Sektor mit Länge und Hindernissen
	struct Sector
	{
		Sector(int len) : length(len) {}
		int length;
		std::vector<Obstacle> obstacles;
	};
	// Leerer Sektor zum Starten
	Sector* emptySector;

	// Alle geladenen Sektoren
	std::vector<Sector*> sectors;
	// Aktive Sektoren
	std::deque<Sector*> sectorQue;
};
