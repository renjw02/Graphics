#pragma once

class Color 
{
public:
	float R;
	float G;
	float B;
	Color() : R(0), G(0), B(0) {}

	Color(float r, float g, float b) : R(r), G(g), B(b) {}

	Color operator+(const Color& b)
	{
		Color c;
		c.R = R + b.R;
		c.G = G + b.G;
		c.B = B + b.B;
		return c;
	}

	Color operator-(const Color& b)
	{
		Color c;
		c.R = R - b.R;
		c.G = G - b.G;
		c.B = B - b.B;
		return c;
	}

	Color operator*(const float& b)
	{
		Color c;
		c.R = R * b;
		c.G = G * b;
		c.B = B * b;
		return c;
	}

	Color operator/(const float& b)
	{
		Color c;
		c.R = R / b;
		c.G = G / b;
		c.B = B / b;
		return c;
	}
};