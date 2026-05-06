#pragma once

#include"Geometry.h"

class Sphere : public Geometry
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_radius">半径</param>
	Sphere(const VECTOR& _pos, const float _radius);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	Sphere(const Sphere& _copyBase, const VECTOR& _pos);

	//デストラクタ
	~Sphere(void)override;

	//描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Line& _line)override;

	//ヒット後の処理
	void HitAfter(void)override;

private:
};