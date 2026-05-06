#pragma once

#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Model;
class Cube;
class Sphere;
class Capsule;
class Line;

class Geometry
{
public:

	//バウンディングボックス
	struct OBB
	{
		VECTOR vMin;
		VECTOR vMax;
		VECTOR axis[3];
	};

	//通常色
	static constexpr int NORMAL_COLOR = 0x000000;

	/// @brief デストラクタ
	/// @param  
	virtual~Geometry(void) = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void) = 0;

	//各種当たり判定
	virtual const bool IsHit(Geometry& _geometry) = 0;		//ジオメトリ
	virtual const bool IsHit(Model& _model) = 0;			//モデル
	virtual const bool IsHit(Cube& _cube) = 0;				//キューブ
	virtual const bool IsHit(Sphere& _sphere) = 0;			//球
	virtual const bool IsHit(Capsule& _capsule) = 0;		//カプセル
	virtual const bool IsHit(Line& _line) = 0;				//線
	
	/// @brief 衝突後の処理
	/// @param  
	virtual void HitAfter(void);

	/// @brief  親の座標の取得
	/// @param  
	/// @return 親の座標
	inline const VECTOR GetColPos(void)const { return pos_; }

	/// @brief クォータニオン角度を返す
	/// @param  
	/// @return クォータニオン角度
	inline const Quaternion& GetColRot(void)const { return quaRot_; }


	/// @brief 1つ目の点の座標を取得
	/// @param  
	/// @return 
	inline const VECTOR GetLocalPosPoint1(void)const { return localPosPoint1_; }

	/// @brief 2つ目の点の座標を取得
	/// @param  
	/// @return 
	inline const VECTOR GetLocalPosPoint2(void)const { return localPosPoint2_; }


	/// @brief 回転済みの1つ目の点の座標を取得
	/// @param  
	/// @return 
	inline const VECTOR GetPosPoint1(void) const { return GetRotPos(localPosPoint1_); }
	
	/// @brief 回転済みの2つ目の点の座標を取得
	/// @param  
	/// @return 
	inline const VECTOR GetPosPoint2(void) const { return GetRotPos(localPosPoint2_); }

	/// @brief 中心座標の取得
	/// @param  
	/// @return 
	const VECTOR GetCenter(void)const;


	/// @brief 1つ目の点の座標のセット
	/// @param _pos 1つ目の点の座標
	inline void SetLocalPosPoint1(const VECTOR _pos) { localPosPoint1_ = _pos; }

	/// @brief 2つ目の点の座標の設定
	/// @param _pos  2つ目の点の座標
	inline void SetLocalPosPoint2(const VECTOR _pos) { localPosPoint2_ = _pos; }


	/// @brief 半径の取得
	/// @param  
	/// @return 半径
	inline const float GetRadius(void)const { return radius_; }

	/// @brief 半径の設定
	/// @param _radius 半径
	inline void SetRadius(const float _radius) { radius_ = _radius; }

	/// @brief 当たった時の線の情報取得
	/// @param  
	/// @return 
	inline const MV1_COLL_RESULT_POLY& GetHitLineInfo(void)const { return hitLineInfo_; }

	/// @brief 当たった時の情報取得
	/// @param  
	/// @return 
	inline const MV1_COLL_RESULT_POLY_DIM& GetHitInfo(void)const { return hitInfo_; }

	/// @brief 親モデルIDの取得
	/// @param  
	/// @return 
	inline const int GetParentModel(void)const { return parentModelId_; }

	/// @brief 当たった時の線の情報設定
	/// @param _hitLineInfo 当たった時の線の情報
	inline void SetHitLineInfo(const MV1_COLL_RESULT_POLY _hitLineInfo) { hitLineInfo_ = _hitLineInfo; }

	/// @brief 当たった時の情報設定
	/// @param _hitInfo 当たった時の情報
	inline void SetHitInfo(MV1_COLL_RESULT_POLY_DIM _hitInfo) { std::swap(hitInfo_, _hitInfo); }


	/// @brief 回転バウンティボックスの取得
	/// @param  
	/// @return 回転バウンティボックス
	inline const OBB& GetObb(void)const { return obb_; }


	/// @brief 箱の最小地点の取得
	/// @param  
	/// @return 箱の最小地点
	inline const VECTOR GetVecMin(void)const { return obb_.vMin; }

	/// @brief 箱の最大地点の取得
	/// @param  
	/// @return 箱の最大地点
	inline const VECTOR GetVecMax(void)const { return obb_.vMax; }

	/// @brief 回転バウンティボックスの設定
	/// @param _obb 回転バウンティボックス
	inline void SetObb(const OBB& _obb) { obb_ = _obb; }

	/// @brief 箱の最小地点の設定
	/// @param _min 箱の最小地点
	inline void SetVecMin(const VECTOR& _min) { obb_.vMin = _min; }

	/// @brief 箱の最大地点の設定
	/// @param _max 箱の最大地点
	inline void SetVecMax(const VECTOR& _max) { obb_.vMax = _max; }

	/// @brief サイズの半分の設定
	/// @param _halfSize サイズの半分
	inline void SetHalfSize(const VECTOR& _halfSize);

protected:

	/// <summary>
	/// コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Geometry(const VECTOR& _pos, const Quaternion& _rot, const float& _radius,
		const VECTOR& _localPosPoint1, const VECTOR& _localPosPoint2,
		const OBB& _obb, const int& _parentModelId);

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;

	//親の座標
	const VECTOR& pos_;			
	//親の回転
	const Quaternion& quaRot_;	

	// 半径
	float radius_;			
	MV1_COLL_RESULT_POLY hitLineInfo_;	//当たった時の情報(線)
	MV1_COLL_RESULT_POLY_DIM hitInfo_;	//当たった時の情報(球、カプセル)

	//線
	VECTOR localPosPoint1_;	//1つ目の点の座標
	VECTOR localPosPoint2_;	//2つ目の点の座標


	//キューブ
	OBB obb_;			//回転バウンディングボックス

	//モデル
	int parentModelId_;					//親のモデルID
};