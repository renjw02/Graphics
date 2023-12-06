#pragma once
#include <string>

class BMP
{

public:
    int w;
    int h;
    unsigned char* data;
    BMP();
    BMP(std::string filename);
    ~BMP();
};

