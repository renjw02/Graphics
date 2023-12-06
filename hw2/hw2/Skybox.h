#pragma once
#include "BMP.h"

class Skybox
{
	BMP* env[6];
	BMP* env1[6];

public:
	int type;
	void initSky(unsigned int* texture);
	void invert(unsigned int* texture, int type);
	void renderEnvironment(unsigned int* texture);
};

