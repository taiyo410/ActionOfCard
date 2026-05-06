#include "IntVector3.h"

IntVector3::IntVector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}

IntVector3::IntVector3(int vX, int vY, int vZ)
{
	x = vX;
	y = vY;
	z = vZ;
}

IntVector3::~IntVector3(void)
{
}

const IntVector3 IntVector3::operator+(const IntVector3 _value)const
{
	return { x + _value.x , y + _value.y, z + _value.z};
}

void IntVector3::operator+=(const IntVector3 _value)
{
	x += _value.x;
	y += _value.y;
	z += _value.z;
}

const IntVector3 IntVector3::operator-(const IntVector3 _value)const
{
	return { x - _value.x , y - _value.y, z - _value.z };
}

void IntVector3::operator-=(const IntVector3 _value)
{
	x -= _value.x;
	y -= _value.y;
	z -= _value.z;
}

const IntVector3 IntVector3::operator*(const int _value)const
{
	return { x * _value , y * _value, z * _value };
}

void IntVector3::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
	z *= _value;
}

const IntVector3 IntVector3::operator/(const int _value)const
{
	return { x / _value , y / _value, z / _value };
}

void IntVector3::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
	z /= _value;
}
