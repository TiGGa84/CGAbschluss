
#ifndef Track_hpp
#define Track_hpp

#include "BaseModel.h"
#include "Model.h"

#include <vector>
#include <deque>
#include <string>
#include <random>

class Track : public BaseModel
{
public:
	Track(Model* model, double speed, int renderLimit);
	virtual ~Track();
	virtual void draw(const BaseCamera& Cam);

	void update(double dtime);
protected:
	void readSector(std::string Filepath);
	void cleanSectorQue(double offsetToRemove);
	void fillSectorQue(double offsetToFill);

	Model* model;
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
	Sector* emptySector;

	// Alle geladenen Sektoren
	std::vector<Sector*> sectors;
	// Aktive Sektoren
	std::deque<Sector*> sectorQue;
};


#endif
