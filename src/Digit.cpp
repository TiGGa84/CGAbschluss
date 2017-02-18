#include "Digit.h"

Digit::Digit(float x, float y, float scaleX, float scaleY, int num) {
	if (num > 9) num = 0;
	VB.begin();

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0 + ((float)num / 10), 1);
	VB.addVertex(x, y, 0);

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0.1f + ((float)num / 10) , 1);
	VB.addVertex(x + scaleX, y, 0);

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0 + ((float)num / 10), 0);
	VB.addVertex(x, y + scaleY, 0);

	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0.1f + ((float)num / 10), 0);
	VB.addVertex(x + scaleX, y + scaleY, 0);

	VB.end();

	IB.begin();
	
	IB.addIndex(0);
	IB.addIndex(1);
	IB.addIndex(2);

	IB.addIndex(2);
	IB.addIndex(1);
	IB.addIndex(3);

	IB.end();
}

void Digit::draw(const BaseCamera& Cam) {
	BaseModel::draw(Cam);

	VB.activate();
	IB.activate();

	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

	IB.deactivate();
	VB.deactivate();
}