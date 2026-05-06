#pragma once
#include "./Vector2F.h"

struct Vector2
{
	int x;
	int y;

	/// @brief 正規化
	/// @param  
	/// @return 
	Vector2 Normalize(void) const;

	/// @brief 長さを求める
	/// @param  
	/// @return 
	float Length(void) const;

	/// @brief Vector2同士の比較
	/// @param value1 ベクトル1
	/// @param value2 ベクトル2
	/// @return 
	static bool IsVector2(const Vector2 value1, const Vector2 value2);
	
	/// @brief Vector2同士の値が等しいか比較
	/// @param value1 ベクトル1
	/// @param value2 ベクトル2
	/// @return 
	static bool IsSameVector2(const Vector2 value1, const Vector2 value2);

	//Vector2同士の演算
	const Vector2 operator+(const Vector2 _value)const;
	void operator+=(const Vector2 _value);
	const Vector2 operator-(const Vector2 _value)const;
	void operator-=(const Vector2 _value);
	const Vector2 operator*(const Vector2 _value)const;
	void operator*=(const Vector2 _value);
	const Vector2 operator/(const Vector2 _value)const;
	void operator/=(const Vector2 _value);

	//一つの数での演算
	template<typename T>
	const Vector2 operator+(const T _value)const
	{
		return{ x + _value,y + _value };
	}
	template<typename T>
	void operator+=(const T _value)
	{
		x += _value;
		y += _value;

	}
	template<typename T>
	const Vector2 operator-(const T _value)const
	{
		return { static_cast<int>(x - _value),static_cast<int>(y - _value) };
	}
	template<typename T>
	void operator-=(const T _value)
	{
		x -= _value;
		y -= _value;
	}
	template<typename T>
	const Vector2 operator*(const T _value)const
	{
		return { static_cast<int>(x * _value),static_cast<int>(y * _value) };
	}
	template<typename T>
	void operator*=(const T _value)
	{
		x *= _value;
		y *= _value;
	}
	template<typename T>
	const Vector2 operator/(const T _value)const
	{
		return { static_cast<int>(x / _value),static_cast<int>(y / _value) };
	}
	template<typename T>
	void operator/=(const T _value)
	{
		x /= _value;
		y /= _value;
	}
};