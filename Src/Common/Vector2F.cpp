#include <cmath>
#include "Vector2.h"
#include "Vector2F.h"


//Vector2F“¯Žm‚Ì”äŠr
bool Vector2F::IsVector2F(Vector2F value1, Vector2F value2)
{
	if (value1.x < value2.x &&
		value1.y < value2.y)
	{
		return true;
	}

	return false;
}

bool Vector2F::IsSameVector2F(const Vector2F value1, const Vector2F value2)
{
	if (value1.x == value2.x && value1.y == value2.y)
	{
		return true;
	}
	return false;
}


const Vector2F Vector2F::operator+(const Vector2F _value)const
{
	return { x + _value.x,y + _value.y };
}
void Vector2F::operator+=(const Vector2F _value)
{
	x += _value.x;
	y += _value.y;
}
const Vector2F Vector2F::operator-(const Vector2F _value)const
{
	return { x - _value.x,y - _value.y };
}
void Vector2F::operator-=(const Vector2F _value)
{
	x -= _value.x;
	y -= _value.y;
}
const Vector2F Vector2F::operator*(const Vector2F _value)const
{
	return { x * _value.x,y * _value.y };
}
void Vector2F::operator*=(const Vector2F _value)
{
	x*= _value.x;
	y*= _value.y;
}
const Vector2F Vector2F::operator/(const Vector2F _value)const
{
	return { x / _value.x,y / _value.y };
}
void Vector2F::operator/=(const Vector2F _value)
{
	x /= _value.x;
	y /= _value.y;
}