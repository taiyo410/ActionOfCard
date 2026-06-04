#pragma once
#include "EnemyCardAttackBase.h"
class EnemyCardAttackJump :
    public EnemyCardAttackBase
{

public:

	/// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
	/// @param _charaObj キャラクターオブジェクト
	/// @param _cardPresent カードプレゼンター
	EnemyCardAttackJump(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent);

	/// @brief デストラクタ
	/// @param  
	~EnemyCardAttackJump(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief アクションで使う変数の初期化
	/// @param  
	void InitAttack(void) override;

	/// @brief 攻撃アクションの内容
	/// @param  
	void AttackUpdate(void) override;

	/// @brief 遷移時の開放
	/// @param  
	void AttackRelease(void) override;

	/// @brief アクションデータのロード
	/// @param _data 
	void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

#pragma region 外部ファイル読み込み
	//溜め状態関連
	float chargeEffScale_;				//溜め状態エフェクト大きさ
	float chargeTime_;					//溜め時間
	float chargeAnimLoopStartStep_;		//溜めアニメーションループ初期ステップ
	float chargeAnimLoopEndStep_;		//溜めアニメーションループ終わりステップ
	float chargeAnimLoopSpd_;			//溜めアニメーションループ速度
	float chargeCameraShakeLimit_;		//溜め時のカメラシェイク可動範囲
	float jumpAnimEndStep_;				//ジャンプ終了アニメーションステップ

	//攻撃状態
	VECTOR atkLocalPos_;				//攻撃ローカル座標
	float atkStartRadius_;				//攻撃開始時の攻撃範囲
	float atkEndRadius_;				//攻撃終了時の攻撃範囲
	float atkExpandSpd_;				//攻撃範囲の広がる速度
	float atkExpandTime_;				//攻撃範囲の広がる時間
	float atkAnimLoopStartStep_;		//攻撃アニメーションループ開始ステップ
	float atkAnimLoopEndStep_;			//攻撃アニメーションループ終了ステップ
	float atkAnimLoopSpd_;				//攻撃アニメーションループ速度
	float atkCameraShakeLimit_;			//攻撃時のカメラシェイク範囲
	float atkEffScale_;					//攻撃エフェクトの大きさ
#pragma endregion

#pragma region メンバー変数
	std::unique_ptr<Easing>easing_;		//イージング
	float jumpChargeCnt_;				//ジャンプチャージカウント

	//エフェクト再生番号
	int jumpChargeEffNum_;		//ジャンプチャージ	
	int blastEffNum_;			//ドーム攻撃エフェクト
#pragma endregion



};

