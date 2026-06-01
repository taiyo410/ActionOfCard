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

    //攻撃半径広がるスピード
    static constexpr float JUMP_ATK_COL_SPD = 5.0f;

    //広がる時間
    static constexpr float JUMP_ATK_CNT_MAX = 2.5f;

    //ジャンプチャージ時間
    static constexpr float JUMP_CHARGE_TIME = 6.0f;

    //ジャンプ攻撃アニメーション
    static constexpr float JUMP_ANIM_END = 52.0f;

    //ジャンプ攻撃ローカル座標
    static constexpr VECTOR JUMP_ATK_LOCAL = { 0.0f,100.0f,0.0f };

    //ジャンプ攻撃範囲
    static constexpr float JUMP_ATK_GOAL_RADIUS = 2500.0f;

    //ジャンプ溜めカメラシェイク
    static constexpr float JUMP_CHARGE_SHAKE_LIMIT = 10.0f;
    static constexpr float JUMP_ATK_SHAKE_LIMIT = 30.0f;

    //ジャンプアニメーションループ(溜めアニメーション)
    static constexpr float JUMP_CHARGE_ANIM_LOOP_START = 12.0f;
    static constexpr float JUMP_CHARGE_ANIM_LOOP_END = 13.0f;
    static constexpr float JUMP_ATK_ANIM_LOOP_SPEED = 5.0f;

    //ジャンプアニメーションループ(攻撃アニメーション)
    static constexpr float JUMP_ATK_ANIM_LOOP_START = 53.0f;
    static constexpr float JUMP_ATK_ANIM_LOOP_END = 69.0f;

    //ジャンプアニメーションループ速度
    static constexpr float JUMP_ATK_ANIM_ATTACK_LOOP_SPEED = 10.0f;

    //ジャンプチャージ時のカメラシェイク可動範囲
    static constexpr float JUMP_CHARGE_CAMERA_SHAKE_LIMIT = 10.0f;

    //ジャンプ攻撃時のカメラシェイク可動範囲
    static constexpr float JUMP_ATTACK_CAMERA_SHAKE_LIMIT = 30.0f;

    //ジャンプ攻撃初期の半径
    static constexpr float JUMP_ATK_START_RADIUS = 30.0f;

    //エフェクトサイズ
    static constexpr float BLAST_EFF_SCL = 1.4f;            //爆発
    static constexpr float JUMP_CHARGE_EFF_SCL = 500.0f;      //ジャンプチャージ

    //イージング
    std::unique_ptr<Easing>easing_;

	//ジャンプチャージカウント
	float jumpChargeCnt_;

};

