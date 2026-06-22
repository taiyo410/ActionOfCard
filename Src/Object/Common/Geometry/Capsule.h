#pragma once

#include"./Geometry.h"

class Capsule : public Geometry
{

public:

	/// @brief コンストラクタ
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親の回転
	/// @param _localPosTop 上側の相対座標
	/// @param _localPosDown 下側の相対座標
	/// @param _radius 半径
	Capsule(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosTop, const VECTOR _localPosDown, const float _radius);
	
	/// @brief コピーコンストラクタ
	/// @param _copyBase コピー元
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親の回転
	Capsule(const Capsule& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	/// @brief デストラクタ
	/// @param  
	~Capsule(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry) override;			//ジオメトリ
	const bool IsHit(Model& _model) override;				//モデル
	const bool IsHit(Cube& _cube) override;					//キューブ
	const bool IsHit(Sphere& _sphere) override;				//球
	const bool IsHit(Capsule& _capsule) override;			//カプセル
	const bool IsHit(Line& _line) override;					//ライン
	const bool IsHit(Plane& _line) override;				//プレーン

	/// @brief ヒット後の処理
	/// @param  
	void HitAfter(void)override;

private:
};
