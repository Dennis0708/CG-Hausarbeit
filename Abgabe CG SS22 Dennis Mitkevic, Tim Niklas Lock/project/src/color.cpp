#include "color.h"
#include <assert.h>

Color::Color()
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
}

Color::Color(float r, float g, float b)
{
	if ((r < 0 && r >= 1) || (g < 0 && g >= 1) || (b < 0 && b >= 1)) {
		cerr << "RGB werte muessen kleiner 1 und groesser oder gleich 0 sein";
	}
	else {
		this->R = r;
		this->G = g;
		this->B = b;
	}
}

Color Color::operator*(const Color& c) const
{
	Color ergebnis = Color();
	ergebnis.R = this->R * c.R;
	ergebnis.G = this->G * c.G;
	ergebnis.B = this->B * c.B;
	return ergebnis;
}

Color Color::operator*(const float Factor) const
{
	Color ergebnis = Color();
	ergebnis.R = this->R * Factor;
	ergebnis.G = this->G * Factor;
	ergebnis.B = this->B * Factor;
	return ergebnis;
}

Color Color::operator+(const Color& c) const
{
	Color ergebnis = Color();
	ergebnis.R = this->R + c.R;
	ergebnis.G = this->G + c.G;
	ergebnis.B = this->B + c.B;
	return ergebnis;
}

Color& Color::operator+=(const Color& c)
{
	this->R += c.R;
	this->G += c.G;
	this->B += c.B;
	return *this;
}