#pragma once
#include "Geometry.h"
class ExternalLine :
    public Geometry
{
public:

	/// @brief 2点間をつなぐオブジェクトの線分
	/// @param _objPos1 1つめの物体の座標
	/// @param _objPos2 2つめの物体の座標
	ExternalLine(const VECTOR& _objPos1, const VECTOR& _objPos2);

	/// @brief デストラクタ
	/// @param  
	~ExternalLine(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule) override;
	const bool IsHit(Line& _line) override;
	const bool IsHit(ExternalLine& _externalLine) override;
	const bool IsHit(Plane& _plane) override;

	const VECTOR GetPosPoint1(void)const override;
	const VECTOR GetPosPoint2(void)const override;

private:
	// 参照で持つ場合の追加メンバー
	const VECTOR& externalPoint1_;
	const VECTOR& externalPoint2_;
};

