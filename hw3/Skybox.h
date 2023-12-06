#pragma once

#include "BMP.h"

class Skybox
{
	BMP* env[6];
public:
	void initSky(unsigned int &texture);
	void renderEnvironment(unsigned int &texture);
};

