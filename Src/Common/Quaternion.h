#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{

public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	/// @brief コンストラクタ
	/// @param  
	Quaternion(void);

	/// @brief コンストラクタ
	/// @param rad ラジアン角度
	Quaternion(const VECTOR& rad);

	/// @brief コンストラクタ
	/// @param w クォータニオンw
	/// @param x クォータニオンx
	/// @param y クォータニオンy
	/// @param z クォータニオンz
	Quaternion(double w, double x, double y, double z);

	/// @brief デストラクタ
	/// @param  
	~Quaternion(void);

	/// @brief オイラー角からクォータニオンへ変換
	/// @param rad オイラー角(ラジアン)
	/// @return 
	static Quaternion Euler(const VECTOR& rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	/// @brief クォータニオンの合成
	/// @param q1 クォータニオン角度1
	/// @param q2 クォータニオン角度2
	/// @return 
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	/// @brief 指定軸を指定角分、回転させる
	/// @param rad 指定角(ラジアン)
	/// @param axis 指定軸
	/// @return 
	static Quaternion AngleAxis(double rad, VECTOR axis);

	/// @brief 座標を回転させる
	/// @param q クォータニオン角度
	/// @param pos 座標
	/// @return 
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);
	VECTOR PosAxis(VECTOR pos) const;

	/// @brief クォータニオンからオイラー角へ変換
	/// @param q クォータニオン
	/// @return 
	static VECTOR ToEuler(const Quaternion& q);
	VECTOR ToEuler(void) const;

	// クォータニオンから行列へ変換
	
	/// @brief クォータニオンから行列へ変換
	/// @param q クォータニオン角度
	/// @return 
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix(void) const;

	/// @brief ベクトルからクォータニオンに変換
	/// @param dir ベクトル
	/// @return 
	static Quaternion LookRotation(VECTOR dir);
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	/// @brief 行列からクォータニオンに変換
	/// @param mat 変換したい行列
	/// @return 
	static Quaternion GetRotation(MATRIX mat);
	
	/// @brief 前方ベクトルを取得
	/// @param  
	/// @return 
	VECTOR GetForward(void) const;

	/// @brief 後方ベクトルを取得
	/// @param  
	/// @return 
	VECTOR GetBack(void) const;

	/// @brief 右方ベクトルを取得
	/// @param  
	/// @return 
	VECTOR GetRight(void) const;

	/// @brief 左方ベクトルを取得
	/// @param  
	/// @return 
	VECTOR GetLeft(void) const;

	/// @brief 上方ベクトルを取得
	/// @param  
	/// @return 
	VECTOR GetUp(void) const;

	/// @brief 下方ベクトルを取得
	/// @param  
	/// @return 
	VECTOR GetDown(void) const;

	/// @brief 内積
	/// @param q1 クォータニオン角度1
	/// @param q2 クォータニオン角度2
	/// @return 
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;

	/// @brief 正規化
	/// @param q クォータニオン角度
	/// @return 
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized(void) const;
	void Normalize(void);

	// 逆クォータニオン
	Quaternion Inverse(void) const;

	// 球面補間
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);

	/// @brief ２つのベクトル間の回転量を取得する
	/// @param fromDir 始点ベクトル
	/// @param toDir 終点ベクトル
	/// @return ２つのベクトル間の回転量
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);

	/// @brief 2つのクォータニオン間の補完
	/// @param from 始点クォータニオン
	/// @param to 終点ベクトル
	/// @param maxDegreesDelta 1回の更新で回転できる最大角度（度単位）
	/// @return 球面線形補間で回転を近づけた時の角度
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);

	/// @brief 2つのクォータニオン間の角度を取得する
	/// @param q1 クォータニオン角度1
	/// @param q2 クォータニオン角度2
	/// @return 2つのクォータニオン間の角度
	static double Angle(const Quaternion& q1, const Quaternion& q2);

	/// @brief 2つのクォータニオンを球面線形補間する（補間値の制限なし）
	/// @param a 補間開始の回転（現在の向き）
	/// @param b 補間終了の回転（目標の向き）
	/// @param t 補間係数（0でa、1でb。0未満や1以上も指定可能）
	/// @return 補間結果のQuaternion
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);

	/// @brief 回転を持たない単位クォータニオンを返す
	/// @param  
	/// @return 初期値のクォータニオン
	static Quaternion Identity(void);

	/// @brief クォータニオンの長さを取得
	/// @param  
	/// @return クォータニオンの長さ
	double Length(void) const;

	/// @brief 2乗のクォータニオンの長さを取得
	/// @param  
	/// @return 2乗のクォータニオンの長さ
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	/// @brief クォータニオンをラジアン指定角と軸に変換
	/// @param angle ラジアン指定角
	/// @param axis 指定軸
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	//演算子
	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
};