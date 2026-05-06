#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class UtilityDraw
{
public:

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	/// @brief 点線を描画する
	/// @param sPos 始点
	/// @param ePos 終点
	/// @param color 色
	/// @param len 間隔
	static void DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len = 50.0f);

	/// @brief センター座標に文字列を描画する
	/// @param x 
	/// @param y 
	/// @param str 
	/// @param color 
	/// @param fontHandle 
	static void DrawStringCenter(const int _centerX, const int _centerY, const std::wstring _str, unsigned int color, const int fontHandle);
};

