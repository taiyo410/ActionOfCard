#pragma once
#include<DxLib.h>
#include <string>
#include <map>
#include "../Common/Easing.h"

class Easing;
class SceneManager;

class AnimationController
{
	
public :

	//アニメーションの可変パラメータ
	struct ANIMATION_VARIABLE
	{
		float speed = 0.0f;					//アニメーション速度
		float totalTime = 0.0f;				//トータル時間
		float detachSpeed = 1.0f;			//アニメーション終了後の速度
		bool isLoop = true;					//ループするか
		bool isMidLoop=false;				//途中ループフラグ
		float switchLoopReverse = 1.0f;			//途中ループの切り替え用
		float step = 0.0f;			//ステップ
		// アニメーション終了後に繰り返すループステップ
		float stepEndLoopStart=0.0f;
		float stepEndLoopEnd=0.0f;
		float endLoopSpeed=0.0f;
		VECTOR invalidPos = {};		//座標移動無効化用
		bool isStop;				// アニメーションを止めたままにする
	};

	// アニメーションデータ
	struct ANIMATION_PARAMETER
	{
		int model = -1;				//モデルID
		int attachNo = -1;			//アタッチ番号
		int animIndex = 0;			//アニメーション番号
		VECTOR invalidPos = {};		//座標移動無効化用
		VECTOR firstPos = {};		//移動量格納
		VECTOR movePow = {};		//座標移動無効化用
		bool isStop;				// アニメーションを止めたままにする
		int isPriority = false;		//優先されているか
		float blendRate = 0.0f;		//ブレンド率

		ANIMATION_VARIABLE variable;		//アニメーションの可変パラメータ
	};

	/// @brief コンストラクタ
	/// @param _modelId モデルID
	/// @param _hipNum 尻のボーン番号
	AnimationController(void);

	/// @brief デストラクタ
	/// @param  
	~AnimationController(void);

	/// @brief アニメーションパラメータの読み込み
	/// @param animName 
	void LoadData(const std::string& animName);

	/// @brief アニメーション追加
	/// @param type 
	/// @param speed 
	/// @param modelId 
	void Add(int type, int modelId);

	/// @brief ブレンドアニメーション用再生
	/// @param type アニメーションタイプ
	/// @param isLoop ループするかしないか
	/// @param startStep アニメーションスタート位置
	/// @param endStep アニメーション終了位置
	/// @param isStop 
	/// @param isForce 
	//void PlayBlend(int type,float animSpeed ,float detachSpeed,bool isLoop = true, 
	//	VECTOR invalidBlendPos = {}, float startStep = 0.0f, float endStep = -1.0f, bool isStop = false);
	void PlayBlend(int type,ANIMATION_VARIABLE animVariable);

	/// @brief アニメーション更新 
	/// @param _spdScl 
	void Update(const float _spdScl=1.0f);

	/// @brief アニメーション終了後に繰り返すループステップ
	/// @param startStep スタート位置
	/// @param endStep 終了位置
	/// @param speed アニメーション速度
	void SetEndLoop(int type,float startStep, float endStep, float speed);

	/// @brief アニメーションの途中からのループ再生
	/// @param startStep スタート位置
	/// @param endStep 終了位置
	/// @param _spd アニメーション速度
	void SetMidLoop(int type, const float startStep,const float endStep,float _spd);

	/// @brief アニメーション途中ループ終了
	/// @param _spd 元に戻した時のアニメーション速度
	void SetEndMidLoop(int type, const float _spd);

	/// @brief 再生中のアニメーションタイプの取得
	/// @param  
	/// @return 
	int GetPlayType(void) const;

	/// @brief アニメーションステップゲッタ
	/// @param  
	/// @return 
	const float GetAnimStep(const int animType)const;

	/// @brief アニメーションスピードセッタ(イージングで、だんだん増やしていくのも可)
	/// @param _spd セットしたいスピード
	/// @param _isEase イージングを使用するか
	/// @param _startSpd イージング使用時の始まりのスピード
	/// @param _t 時間
	/// @param _easeType 使用したいイージングタイプ
	void SetAnimSpeed(int type, const float _spd, const bool _isEase = false, const float _startSpd=0.0f, const float _t = 1.0f, Easing::EASING_TYPE _easeType = Easing::EASING_TYPE::LERP);

	/// @brief // 再生終了
	/// @param  
	/// @return 
	bool IsEnd(int type) const;

	/// @brief フレームの行列ローカル座標のセット
	/// @param _modelId モデルID
	/// @param frameIdx フレーム番号
	/// @param pos フレーム座標
	void SetFrameLocalMatrixPos(const int _modelId, const int frameIdx, VECTOR& pos);

	/// @brief アニメーションのフレーム位置をセット
	/// @param modelId モデルID
	/// @param attachNo アタッチ番号
	/// @param frameIdx フレーム番号
	/// @param pos 座標
	void SetFrameAnimAttachLocalMatrixPos(int modelId, int attachNo, int frameIdx, VECTOR& pos);

	/// @brief モデル情報をセット
	/// @param _modelId モデルId
	/// @param _spineNum 腰のボーン番号
	void SetModelInfo(const int _modelId, const int _spineFrameNum);

	/// @brief フレーム行列情報の取得
	/// @param _modelId モデルID
	/// @param _frameIdx フレーム番号
	/// @param _scl スケール
	/// @param _matRot 行列角度
	/// @param _pos 座標
	void GetFrameLocalMatrix(const int _modelId, int _frameIdx, VECTOR& _scl, MATRIX& _matRot, VECTOR& _pos);

	/// @brief // 指定アニメーションのフレーム(ボーン)のローカル行列を取得し、
			   // 拡大縮小・回転・移動成分に分解して出力する
	/// @param modelId モデルID
	/// @param attachNo アタッチ番号
	/// @param frameIdx フレーム番号
	/// @param scl スケール
	/// @param matRot 行列角度
	/// @param pos 座標
	void GetFrameAnimAttachLocalMatrix(int modelId, int attachNo, int frameIdx, VECTOR& scl, MATRIX& matRot, VECTOR& pos);

	/// @brief 特定のアニメーションがブレンドしているか
	/// @param type アニメーション番号
	/// @return true：ブレンドしている false：ブレンドしていない
	bool IsBlendAnim(const int type) const;

	/// @brief 描画
	/// @param  
	void DrawDebug(void);

private :

	//ヒップフレームの番号
	static constexpr int HIP_FRAME_NO = 0;

	//シーンマネージャ
	SceneManager& scnMng_;

	//状態更新
	std::function<void(const float _spdScl)>stateUpdate_;

	//更新配列
	std::vector<std::function<void(const float _spdScl)>>stateUpdates_;

	//イージング
	std::unique_ptr<Easing>easing_;

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, ANIMATION_PARAMETER> animations_;

	//再生の種類
	int playType_;

	//ブレンドしているか
	bool isBlend_;

	// アニメーションの座標移動を無効カウするためのオフセット
	VECTOR invalidBlendPos_;

	//モデルの腰番号
	int spineFrameNum_;

	//状態別更新
	void UpdateNone(void);		//アニメーションブレンド
	void UpdateBlend(void);		//アニメーションブレンド
	void UpdateNormal(const float _spdScl = 1.0f);	//通常

	//移動量打ち消し
	void FreezeMovementForAnimation(void);

	//アニメーションデタッチ
	void AnimationDettach(const int _type);

};

