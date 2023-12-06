#include "BMP.h"
#include <fstream>

BMP::BMP()
{
    w = 0;
    h = 0;
    data = NULL;
}

BMP::BMP(std::string filename)
{
    // Initialize input stream.
    std::ifstream f(filename.c_str(), std::ios::binary);
    int offset; // No. of bytes to start of image data in input BMP file.

    // Get start point of image data in input BMP file.
    f.seekg(10);
    f.read((char*)&offset, 4);

    // Get image width and height.
    f.seekg(18);
    f.read((char*)&w, 4);
    f.read((char*)&h, 4);

    // Determine the length of padding of the pixel rows
    // (each pixel row of a BMP file is 4-byte aligned by padding with zero bytes).
    int padding = (3 * w) % 4 ? 4 - (3 * w) % 4 : 0;

    // Allocate storage for temporary input file, read in image data from the BMP file, close input stream.
    unsigned char* temp = new unsigned char[(3 * w + padding) * h];
    f.seekg(offset);
    f.read((char*)temp, (3 * w + padding) * h);
    f.close();

    // Set image width and height and allocate storage for image in output RGB file.
    unsigned char* rgb = new unsigned char[3 * w * h];

    // Copy data from temporary input file to output RGB file adjusting for padding and performing BGR to RGB conversion.
    for (int j = 0; j < h; j++)
        for (int i = 0; i < 3 * w; i += 3)
        {
            int tempPos = (3 * w + padding) * j + i;
            int rgbPos = 3 * w * j + i;
            rgb[rgbPos] = temp[tempPos + 2];
            rgb[rgbPos + 1] = temp[tempPos + 1];
            rgb[rgbPos + 2] = temp[tempPos];
        }

    // Set image width and height and allocate storage for image in output RGBA file.
    data = new unsigned char[4 * w * h];

    // Copy image data from output RGB file to output RGBA file, setting all A values to 1.
    for (int j = 0; j < 4 * w * h; j += 4)
    {
        data[j] = rgb[(j / 4) * 3];
        data[j + 1] = rgb[(j / 4) * 3 + 1];
        data[j + 2] = rgb[(j / 4) * 3 + 2];
        data[j + 3] = 0xFF;
    }

    // Release temporary storage and the output RGB file and return the RGBA version.
    delete[] temp;
    delete[] rgb;
}

BMP::~BMP()
{
    if (data != NULL)
        delete[] data;
}