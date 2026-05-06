#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/Quaternion.h"

class Utility3D
{
public:

	//VECTORの初期化
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XZ = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XYZ = { 1.0f, 1.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//反転用
	static constexpr int RESERVE_NUM = -1;

	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);
	
	/// @brief ベクトルの長さ
	/// @param v 求めたいベクトル
	/// @return 
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);

	/// @brief 2つのベクトル間の距離を求める
	/// @param v1 ベクトル1
	/// @param v2 ベクトル2
	/// @return 
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	
	/// @brief 球体同士の衝突判定
	/// @param pos1 座標1
	/// @param radius1 半径1
	/// @param pos2 座標2
	/// @param radius2 半径2
	/// @return 
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	/// @brief 球体とカプセルの衝突判定
	/// @param sphPos 球の座標
	/// @param sphRadius 球の半径
	/// @param capPos1 カプセル1座標
	/// @param capPos2 カプセル2座標
	/// @param capRadius カプセル半径
	/// @return 
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius,
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	/// @brief 2つのベクトルの比較
	/// @param v1 ベクトル1
	/// @param v2 ベクトル2
	/// @return 
	static bool Equals(const VECTOR& v1, const VECTOR& v2);

	/// @brief ベクトルが0になっているか
	/// @param v1 判定したいベクトル
	/// @return 
	static bool EqualsVZero(const VECTOR& v1);

	/// @brief 正規化(2D)
	/// @param v 正規化したいベクトル
	/// @return 
	static VECTOR Normalize(const Vector2& v);

	/// @brief 正規化(3D)
	/// @param v 正規化したいベクトル
	/// @return 
	static VECTOR VNormalize(const VECTOR& v);

	/// @brief 2つのベクトルの間の角度
	/// @param from 始端
	/// @param to 終端
	/// @return 
	static double AngleDeg(const VECTOR& from, const VECTOR& to);
	
	/// @brief とある点からとある点までの移動ベクトルを返す
	/// @param _start 狙う側
	/// @param _goal 向かう先
	/// @param _speed 設定速度(未設定だと、方向ベクトルのみを返す)
	/// @return 向かう先までの移動ベクトル
	static const VECTOR GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed = 1.0f);

	
	/// @brief 対象に向かう回転軸
	/// @param _pos 自身の座標
	/// @param _targetPos 対象の座標
	/// @param _needAxis 必要な回転軸
	/// @return 回転軸
	static const VECTOR GetRotAxisToTarget(const VECTOR _pos, const VECTOR _targetPos, const VECTOR _needAxis = AXIS_XYZ);
	
	/// @brief 座標を足して回転
	/// @param _followPos 追従対象の座標
	/// @param _followRot 追従対象の角度
	/// @param _localPos 相対座標
	/// @return ローカルを足した後の座標
	static const VECTOR AddPosRotate(const VECTOR& _followPos, const Quaternion& _followRot, const VECTOR& _localPos);
	
	/// @brief 目的の座標を超えたか
	/// @param _startPos 初期座標
	/// @param _goalPos 目的の座標
	/// @param _mydir 自分の方向
	/// @return 
	bool IsBeyondGoalPos(const VECTOR _startPos, const VECTOR _goalPos, const VECTOR _mydir);

	/// @brief ベクトルの反転
	/// @param _vec 反転したいベクトル
	/// @return 
	static const VECTOR ReverseValue(const VECTOR _vec);
};

