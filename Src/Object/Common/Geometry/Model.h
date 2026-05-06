#pragma once

#include"Geometry.h"

class Model : public Geometry
{
public:
	
	/// @brief コンストラクタ
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親の回転
	/// @param _modelId 追従する親のモデルID
	Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId);
	
	/// @brief コピーコンストラクタ
	/// @param _copyBase コピー元
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親のモデルID
	Model(const Model& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	//デストラクタ
	~Model(void)override;

	//描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Line& _line) override;

	//ヒット後の処理
	void HitAfter(void)override;

private:

};
