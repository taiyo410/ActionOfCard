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

	enum class STATE
	{
		CHARGE,
		JUMP,
		DOME_ATK
	};

#pragma region 外部ファイル読み込み
	//溜め状態関連
	float chargeEffScale_ = 0.0f;				//溜め状態エフェクト大きさ
	float chargeTime_;							//溜め時間
	float chargeAnimLoopStartStep_ = 0.0f;		//溜めアニメーションループ初期ステップ
	float chargeAnimLoopEndStep_ = 0.0f;		//溜めアニメーションループ終わりステップ
	float chargeAnimLoopSpd_ = 0.0f;			//溜めアニメーションループ速度
	float chargeCameraShakeLimit_ = 0.0f;		//溜め時のカメラシェイク可動範囲
	float jumpAnimEndStep_ = 0.0f;				//ジャンプ終了アニメーションステップ

	//攻撃状態
	VECTOR atkLocalPos_ = {};					//攻撃ローカル座標
	float atkStartRadius_ = 0.0f;				//攻撃開始時の攻撃範囲
	float atkEndRadius_ = 0.0f;					//攻撃終了時の攻撃範囲
	float atkExpandSpd_ = 0.0f;					//攻撃範囲の広がる速度
	float atkExpandTime_ = 0.0f;				//攻撃範囲の広がる時間
	float atkAnimLoopStartStep_ = 0.0f;			//攻撃アニメーションループ開始ステップ
	float atkAnimLoopEndStep_ = 0.0f;			//攻撃アニメーションループ終了ステップ
	float atkAnimLoopSpd_ = 0.0f;				//攻撃アニメーションループ速度
	float atkCameraShakeLimit_ = 0.0f;			//攻撃時のカメラシェイク範囲
	float atkEffScale_ = 0.0f;					//攻撃エフェクトの大きさ
#pragma endregion

#pragma region メンバー変数
	std::unique_ptr<Easing>easing_;		//イージング
	//状態更新系
	//状態遷移
	std::unordered_map <STATE, std::function<void(void)>> changeState_;
	STATE state_;								//ドーム攻撃状態
	std::function<void(void)> updateState_;		//状態更新	
	float jumpChargeCnt_;						//ジャンプチャージカウント
	//エフェクト再生番号
	int jumpChargeEffNum_;						//ジャンプチャージ	
	int blastEffNum_;							//ドーム攻撃エフェクト
#pragma endregion

#pragma region メンバー関数
	//更新系
	void UpdateCharge(void);	//溜め
	void UpdateJump(void);		//ジャンプ
	void UpdateDomeAttack(void);//ドーム攻撃

	//遷移系
	void ChangeCharge(void);
	void ChangeJump(void);
	void ChangeDomeAttack(void);

	//状態遷移
	void ChangeState(const STATE _state);
#pragma endregion
};

