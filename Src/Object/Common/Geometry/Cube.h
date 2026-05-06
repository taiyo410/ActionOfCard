#pragma once

#include"Geometry.h"

class Cube : public Geometry
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_min">親から見た、箱の最小地点</param>
	/// <param name="_max">親から見た、箱の最大地点</param>
	Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _min, const VECTOR _max);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_halfSize">箱の半分サイズ</param>
	Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _halfSize);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Cube(const Cube& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	/// @brief デストラクタ
	/// @param  
	~Cube(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Line& _line)override;

private:

	//箱の回転情報の取得
	inline const VECTOR GetAxis(const int _num)const { return obb_.axis[_num]; }

	// クォータニオンから回転軸を計算
	void UpdateObbAxis(void);

	// 各頂点の計算（ワールド座標）
	void CalculateVertices(VECTOR outVertices[8]) const;

	//線分とAABBの最短距離の二乗計算
	float ClosestSegmentAABB(const VECTOR& segA, const VECTOR& segB, const VECTOR& aabbMin, const VECTOR& aabbMax);
};