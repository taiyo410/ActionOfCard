#pragma once

#include "Geometry.h"

class Plane :
    public Geometry
{

public:

	/// @brief コンストラクタ
	/// @param _centerPos 中心座標
	/// @param _rot Quaternion角度
	/// @param _halfW 半分の横幅
	/// @param _halfH 半分の縦幅
	Plane(const VECTOR& _centerPos, const Quaternion& _rot, float _halfW, float _halfH);

	/// @brief 最小最大を指定できるコンストラクタ
	/// @param _centerPos 中心座標
	/// @param _rot 角度
	/// @param min 最小
	/// @param max 最大
	Plane(const VECTOR& _centerPos, const Quaternion& _rot,const VECTOR _centerLocalPos,const VECTOR _localRotRad,const VECTOR& _min, const VECTOR& _max);

	/// @brief 描画
	/// @param  
	void Draw(void) override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule) override;
	const bool IsHit(Line& _line) override;
	const bool IsHit(ExternalLine& _line) override;
	const bool IsHit(Plane& _line) override;

private:

};

