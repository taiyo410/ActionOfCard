#pragma once

#include"Geometry.h"

class Capsule : public Geometry
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_localPosTop">上側の相対座標</param>
	/// <param name="_localPosDown">下側の相対座標</param>
	/// <param name="_radius">半径</param>
	Capsule(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosTop, const VECTOR _localPosDown, const float _radius);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
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

	/// @brief ヒット後の処理
	/// @param  
	void HitAfter(void)override;

private:
};
