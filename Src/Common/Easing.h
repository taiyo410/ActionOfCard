#pragma once
#include<DxLib.h>
#include<functional>
#include"../Common/Vector2F.h"
#include"../Common/Vector2.h"
class Easing
{

public:

	//2点間を定めてイージング
	enum class EASING_TYPE
	{
		LERP,				//線形補間
		LERP_COMEBACK,		//行って戻る補完
		OUT_BACK,			//目的地を少し超えて目的に戻る
		QUAD_IN,			//二次関数(だんだん早く)
		QUAD_OUT,			//二次関数(だんだん遅く)
		QUAD_BACK,			//二次関数(行って戻る)
		QUAD_IN_OUT,		//二次関数(スローインスローアウト)
		QUAD_OUT_IN,		//二次関数(ファストインファストアウト)
		CUBIC_IN,			//三次関数(だんだん早く)
		CUBIC_OUT,			//三次関数(だんだん遅く)
		EXPO,				//指数関数(〇次関数より若干緩やか)
		SIN_BACK,			//サイン(もとに戻ってくる)
		COS_BACK,			//コサイン(もとに戻ってくる)
		ELASTIC_IN,			//前半ばね(だんだん大きく)
		ELASTIC_OUT,		//後半ばね
		BOUNCE,				//跳ねるような動き

		//元の位置に戻すイージング
		ELASTIC_BACK,	//元の位置に戻るばね

	};
	//最終的にスタート位置に戻るイージング
	enum class EASING_RETURN
	{
		ELASTIC,
		EPICYCLOID,
		HYPOCYCLOID,
	};

	struct CURVE_PARAM
	{
		float accel = 1.0f;	//開き具合(公式のa)
		Vector2F graph_vertex = { 0.0f,0.0f };		//軸の頂点(b,c)
	};

	struct TRIG_PARAM
	{
		float amplitude = 1.0f;	//振れ幅(a)
		float lambda = 1.0f;	//波長(λ)
	};

	//半分の割合
	static constexpr float HALF = 0.5f;

	/// @brief コンストラクタ
	/// @param  
	Easing(void);

	//デストラクタ
	~Easing(void);

	/// @brief イージング計算
	/// @param start 初期位置
	/// @param end 終了位置(元の位置に戻すイージングを使用したい場合は動いて良い最大値を代入)
	/// @param t 現在時間
	/// @param type 使用するイージング種類
	/// @return 計算結果
	int EaseFunc(const int start, const int end, const float t,const EASING_TYPE type);
	float EaseFunc(const float start, const float end, const float t,const EASING_TYPE type);
	double EaseFunc(const double start, const double end, const float t,const EASING_TYPE type);
	Vector2F EaseFunc(const Vector2F& start, const Vector2F& end, const float t,const EASING_TYPE type);
	Vector2 EaseFunc(const Vector2& start, const Vector2& end, const float t, const EASING_TYPE type);
	VECTOR EaseFunc(const VECTOR& start, const VECTOR& end, const float t, const EASING_TYPE type);
	COLOR_F EaseFunc(const COLOR_F& start, const COLOR_F& end, const float t, const EASING_TYPE type);

	/// @brief 角度の最終計算(360度などの制限があるため、別で処理する)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @param type 使用するイージング種類
	/// @return 計算結果
	float EaseFuncDeg(float& start, float& end, const float t, const EASING_TYPE type);
	double EaseFuncDeg(double& start, double& end, const float t, const EASING_TYPE type);
	float EaseFuncRad(float& start, float& end, const float t, const EASING_TYPE type);

private:

	//イージングの最大値
	static constexpr float EASING_MAX = 1.0f;

	//イージング中央値
	static constexpr float EASING_HALF = 0.5f;

	//角度の最大値(デグリー)
	static constexpr double DEG_MAX = 360.0;

	//半円の最大値(デグリー)
	static constexpr double HALF_DEG_MAX = 180.0;

	//角度の最大値(ラジアン)
	static constexpr float RAD_MAX = DX_TWO_PI_F;

	//半円の最大値(ラジアン)
	static constexpr float HALF_RAD_MAX = DX_PI_F;

	//イージング関数テーブル
	std::unordered_map<EASING_TYPE, std::function<float(float)>>easingFuncTable_;

	//二次関数の計算
	const float CalcQuadFunc(const CURVE_PARAM curve,const float t) const;
	//三次関数の計算
	const float CalcCubicFunc(const CURVE_PARAM curve,const float t) const;

	//三角関数の計算
	const float CalcSinFunc(const TRIG_PARAM trig, const float t) const;
	const float CalcCosFunc(const TRIG_PARAM trig, const float t) const;

	/// @brief 線形補完
	/// @param t 時間
	/// @return 
	float Lerp(const float t);
	float LerpBack(const float t);		//戻る補完

	float OutBack(const float t);		//目的地を少し超えて戻る

	//---------------------------------------------------------------------------------------------
	//二次関数系
	//---------------------------------------------------------------------------------------------
	// 二次関数的な動き(だんだん早く)
	float EaseQuadIn(const float t);

	// 二次関数的な動き(だんだん遅く)
	float EaseQuadOut(const float t);

	//二次関数的な動き(元に戻る)
	float EaseQuadBack(const float t);

	//二次関数的な動き(スローインスローアウト的な)
	float EaseQuadInOut(const float t);

	//二次関数的な動き(ファストインファストアウト的な)
	float EaseQuadOutIn(const float t);
	//---------------------------------------------------------------------------------------------
	//三次関数(二次関数よりも緩やか)
	//---------------------------------------------------------------------------------------------

	// 三次関数の動き(だんだん早く)
	float EaseCubicIn(const float t);

	// 三次関数の動き(だんだん遅く)
	float EaseCubicOut(const float t);

	//---------------------------------------------------------------------------------------------
	//指数関数
	//---------------------------------------------------------------------------------------------
	// 指数を決めてその動きをする
	float EaseExpo(const float t,const int expo=2);

	//前半ばね
	float EaseInElastic(const float t);

	//後半にばねのような動き 
	float EaseOutElastic(const float t);

	//元に戻ってくるばね
	float EaseBackElastic(const float t);

	//バウンドの動き 
	float EaseBounce(const float t);
	
	//----------------------------------------------------------------------------------------------
	//三角関数
	//----------------------------------------------------------------------------------------------
	//サイン波(初めから早く)
	float EaseSinBack(const float t);
	//コサイン波(遅く始まる)
	float EaseCosBack(const float t);

	//外周を回る(花の形)
	Vector2F EaseEpiCycloid(const Vector2F& start, const float t, const float halfRadiusNum = 4, const float smallRadius = 30);

	//内側を回る(とんがっているお花)
	Vector2F EaseHypoCycloid(const Vector2F& start, const float t, const float halfRadiusNum = 4, const float smallRadius = 30);
};