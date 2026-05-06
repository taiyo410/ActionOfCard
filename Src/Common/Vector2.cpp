#include <cmath>
#include "Vector2.h"
#include "Vector2F.h"

Vector2 Vector2::Normalize(void) const
{
	float len = Length();
	return (len != 0) ? Vector2(static_cast<int>(x / len), static_cast<int>(y / len)) : Vector2(0, 0);
}

float Vector2::Length(void) const
{
	return static_cast<float>(std::sqrt(x * x + y * y));
}

bool Vector2::IsVector2(const Vector2 value1, const Vector2 value2)
{
	if (value1.x < value2.x &&
		value1.y < value2.y)
	{
		return true;
	}

	return false;
}

bool Vector2::IsSameVector2(const Vector2 value1, const Vector2 value2)
{
	if (value1.x == value2.x &&
		value1.y == value2.y)
	{
		return true;
	}

	return false;
}

const Vector2 Vector2::operator+(const Vector2 _value)const
{
	return { x + _value.x,y + _value.y };
}
void Vector2::operator+=(const Vector2 _value)
{
	x += _value.x;
	y += _value.y;
}
const Vector2 Vector2::operator-(const Vector2 _value)const
{
	return { x - _value.x,y - _value.y };
}
void Vector2::operator-=(const Vector2 _value)
{
	x -= _value.x;
	y -= _value.y;
}
const Vector2 Vector2::operator*(const Vector2 _value)const
{
	return { x * _value.x,y * _value.y };
}

void Vector2::operator*=(const Vector2 _value)
{
	x *= _value.x;
	y *= _value.y;
}
const Vector2 Vector2::operator/(const Vector2 _value)const
{
	return { x / _value.x,y / _value.y };
}
void Vector2::operator/=(const Vector2 _value)
{
	x /= _value.x;
	y /= _value.y;
}