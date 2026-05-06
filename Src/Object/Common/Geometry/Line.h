#pragma once

#include"Geometry.h"

class Line : public Geometry
{
public:
	
	/// @brief コンストラクタ
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親の回転
	/// @param _localPosPoint1 1つ目の点の座標
	/// @param _localPosPoint2 2つ目の点の座標
	Line(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosPoint1, const VECTOR _localPosPoint2);

	
	/// @brief コピーコンストラクタ
	/// @param _copyBase コピー元
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親の回転
	Line(const Line& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	// デストラクタ
	~Line(void)override;
	
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

private:

};