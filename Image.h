#ifndef _IMAGE_H_
#define _IMAGE_H_

#pragma once

class Color
{
public:
	float r, g, b;

public:
	Color(float _black = 0) { r = _black; g = _black; b = _black; };
	Color(float _r, float _g, float _b) { r = _r; g = _g; b = _b; };
	void setColor(float _r, float _g, float _b) { r = _r; g = _g; b = _b; };
	void plusColor(Color _c) { r += _c.r; g += _c.g; b += _c.b; };
	Color multNumber(float n) { return Color(r * n, g * n, b * n); };
	Color multSecondColor(Color _c) { return Color(r * _c.r, g * _c.g, b * _c.b); };
	Color &normalize() { r /= 255.f; g /= 255.f; b /= 255.f; return *this; };
	Color &returnInterval() { r *= 255.f; g *= 255.f; b *= 255.f; return *this; };
};

class ImageClass
{
private:
	int		width;
	int		height;
	Color**	pixels;

public:
	ImageClass();
	~ImageClass();
	void createImage(int _width, int _height);
	void setPixelValue(int row, int column, Color _color);
	void saveOutputImage(char* fileName);
};

#endif // _IMAGE_H_

