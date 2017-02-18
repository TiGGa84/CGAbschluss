#include "Score.h"
#include "PhongShader.h"
#include "Digit.h"

#define ASSET_DIRECTORY "../../assets/"

Score::Score(std::list<BaseModel*>* models, float x, float y, float scaleX, float scaleY, unsigned int num) {
	for (int j = 0; j < 10; j++) {
		number[j] = 0;
	}
	int tmp = num;
	int count = 9;
	while (tmp > 0 && count >= 0) {
		number[count] = tmp % 10;
		tmp = tmp / 10;
		count--;
	}

	for (int i = 0; i < 10; i++) {
		BaseModel* pModel = new Digit(x+(i*scaleX), y, scaleX, scaleY, number[i]);
		PhongShader *pPhongShader = new PhongShader();
		pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "Ziffern.png"));
		pModel->shader(pPhongShader, true);
		models->push_back(pModel);
	}

	
}