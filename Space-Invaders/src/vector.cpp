#include "vector.h"
#include <assert.h>
#include <math.h>

#define EPSILON 1e-6

Vector::Vector(float x, float y, float z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

Vector::Vector()
{
	
}

float Vector::dot(const Vector& v) const
{
	return (this->X * v.X) + (this->Y * v.Y) + (this->Z * v.Z);
}

Vector Vector::cross(const Vector& v) const
{
	Vector ergebnis = Vector();
	ergebnis.X = (this->Y * v.Z) - (this->Z * v.Y);
	ergebnis.Y = (this->Z * v.X) - (this->X * v.Z);
	ergebnis.Z = (this->X * v.Y) - (this->Y * v.X);
	return ergebnis;
}

Vector Vector::operator+(const Vector& v) const
{
	Vector ergebnis = Vector();
	ergebnis.X = this->X + v.X;
	ergebnis.Y = this->Y + v.Y;
	ergebnis.Z = this->Z + v.Z;
	return ergebnis;
}

Vector Vector::operator-(const Vector& v) const
{
	Vector ergebnis = Vector();
	ergebnis.X = this->X - v.X;
	ergebnis.Y = this->Y - v.Y;
	ergebnis.Z = this->Z - v.Z;
	return ergebnis;
}

Vector Vector::operator*(float c) const
{
	Vector ergebniss = Vector();
	ergebniss.X = this->X * c;
	ergebniss.Y = this->Y * c;
	ergebniss.Z = this->Z * c;
	return ergebniss;
}

Vector Vector::operator-() const
{
	return Vector(-this->X, -this->Y, -this->Z);
}

Vector& Vector::operator+=(const Vector& v)
{
	this->X += v.X;
	this->Y += v.Y;
	this->Z += v.Z;
	return *this;
}

Vector& Vector::normalize()
{
	return *this = *this * (1 / length());
}

float Vector::length() const
{
	return sqrtf(lengthSquared());
}

float Vector::lengthSquared() const
{
	return this->dot(*this);
}

Vector Vector::reflection(const Vector& normal) const
{
	return *this - (normal * (2.0f * (normal.dot(*this))));
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	Vector n = ((b - a).cross(c - a)).normalize();

	s = ((n.dot(a) - n.dot(*this)) / n.dot(d));

	if (s < EPSILON) {
		return false;
	}

	Vector p = *this + (d * s);

	return triangleArea(a, b, c) + EPSILON >= triangleArea(a, b, p) + triangleArea(a, c, p) + triangleArea(b, c, p);
}

float Vector::triangleArea(const Vector& a, const Vector& b, const Vector& c) const
{
	return ((b - a).cross(c - a)).length() / 2.0f;
}

string Vector::toString() const{
	return "X = " + to_string(X) + ", Y = " + to_string(Y) + ", Z = " + to_string(Z);
}