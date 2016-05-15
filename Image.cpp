#include "Image.h"
#include <fstream>
#include <iostream>

using namespace std;

#pragma warning(disable:4996)

ImageClass::ImageClass()
{
	width = 0;
	height = 0;
}

void ImageClass::createImage(int _width, int _height)
{
	width = _width;
	height = _height;
	pixels = new Color*[width];
	for (int i = 0; i < width; i++)
		pixels[i] = new Color[height];
}

void ImageClass::setPixelValue(int row, int column, Color _color)
{
	if (row >=0 && row < width && column >=0 && column < height)
		pixels[row][column] = _color;
}

ImageClass::~ImageClass()
{
	for (int i = 0; i < width; i++)
		delete[] pixels[i];
	delete[] pixels;
}

void ImageClass::saveOutputImage(char* fileName)
{
	FILE   *outputFile;
	int		fileSize = width * height + 54;

	outputFile = fopen(fileName, "wb");
	if (outputFile == NULL)
	{
		cout << "Cannot create output file" << endl;
		return;
	}

	unsigned char bitmapFileHeader[14] = { 'B', 'M',
		(unsigned char)fileSize, (unsigned char)(fileSize >> 8), (unsigned char)(fileSize >> 16), (unsigned char)(fileSize >> 24),
		0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bitmapInfo[40] = { 40, 0, 0, 0,
		(unsigned char)width, (unsigned char)(width >> 8), (unsigned char)(width >> 16), (unsigned char)(width >> 24),
		(unsigned char)height, (unsigned char)(height >> 8), (unsigned char)(height >> 16), (unsigned char)(height >> 24),
		1, 0, 24, 0 };
	unsigned char endlinePaddingData[3] = { 0, 0, 0 };
	int paddingLen = (4 - (width * 3) % 4) % 4;

	unsigned char imagePixelData[3] = { 0, 0, 0 };


	fwrite(bitmapFileHeader, 1, 14, outputFile);
	fwrite(bitmapInfo, 1, 40, outputFile);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			imagePixelData[0] = (unsigned char)(int)pixels[i][j].b;
			imagePixelData[1] = (unsigned char)(int)pixels[i][j].g;
			imagePixelData[2] = (unsigned char)(int)pixels[i][j].r;
			fwrite(imagePixelData, 1, 3, outputFile);
		}
		fwrite(endlinePaddingData, 1, paddingLen, outputFile);
	}

	fclose(outputFile);
}