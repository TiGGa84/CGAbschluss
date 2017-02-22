#include "Score.h"

#define ASSET_DIRECTORY "../../assets/"

#define ZIFFER_H 75.0f
#define ZIFFER_W 44.0f

Score::Score(float x, float y, float lineheight)
{
	float ratio = ZIFFER_W / ZIFFER_H;
	width = lineheight * ratio;

	rect = new HUDElement(0, 0, width, lineheight, Texture::LoadShared(ASSET_DIRECTORY "Ziffern.png"));
	rect->shader(new HUDShader(), true);
	rect->setTextureScale(0.1f, 1);

	setNumber(0);
	setPosition(x, y);
}

Score::~Score()
{
	delete rect;
}

void Score::setNumber(unsigned int n)
{
	for (int j = 0; j < 10; j++) {
		number[j] = 0;
	}
	int tmp = n;
	int count = 9;
	while (tmp > 0 && count >= 0) {
		number[count] = tmp % 10;
		tmp = tmp / 10;
		count--;
	}
}

void Score::setPosition(float x, float y)
{
	Matrix m;
	m.translation(x, y, 0);
	transform(m);
}

void Score::draw(const BaseCamera & Cam)
{
	for (int i = 0; i < 10; i++) {
		Matrix m;
		// Objekt verschieben
		m.translation(i * width, 0, 0);
		rect->transform(transform() * m);
		// Textur verschieben
		rect->setTextureOffset((float)number[i] / 10.0f, 0);
		rect->draw(Cam);
	}
}
