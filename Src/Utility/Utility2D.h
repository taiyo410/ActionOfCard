#pragma once
#include "../Common/Vector2.h"

class Utility2D
{
public:

	//ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static int SqrMagnitude(const Vector2& v);
	static double Distance(const Vector2& v1, const Vector2& v2);
	
	/// @brief ワールド座標をマップ座標へ
	/// @param worldPos ワールド座標
	/// @param mapSize マップチップ等のサイズ
	/// @return マップ座標
	static Vector2 WorldToMapPos(const Vector2& worldPos, const Vector2& mapSize);
	
	/// @brief マップ座標をワールド座標に
	/// @param mapPos マップ座標
	/// @param mapSize マップチップ等のサイズ
	/// @return ワールド座標
	static Vector2 MapToWorldPos(const Vector2& mapPos, const Vector2& mapSize);
	
	/// @brief 円同士の衝突判定
	/// @param circlePos1 円座標1
	/// @param radius1 円半径1
	/// @param circlePos2 円座標2
	/// @param radius2 
	/// @return 衝突してたらtrue, そうじゃない場合false
	static bool IsHitCircles(const Vector2& circlePos1, const float radius1, const Vector2& circlePos2, const float radius2);

	/// @brief バーグラフ描画
	/// @param pos 座標
	/// @param size 長さ
	/// @param per 割合
	/// @param backColor 背景色
	/// @param fillColor バー本体色
	/// @param offset オフセット
	static void DrawBarGraph(const Vector2& pos, const Vector2& size, const float per, const unsigned int backColor, const unsigned int fillColor, const int offset = 2);

};

