#pragma once
#include<functional>
#include<queue>
#include "../Base/LogicBase.h"
#include "../Base/ActionBase.h"
#include "../Base/CardActionBase.h"

class Easing;
class CardDeck;
class EnemyRock;
class CharacterBase;
class EffectController;

class EnemyCardAction :
    public CardActionBase
{

public:

    //アクションの種類
    enum class ACT_TYPE
    {
        NONE = -1,
        SWIP_ATK, //ひっかき攻撃
        ROAR,     //咆哮
        JUMP_ATK, //攻撃アクション3回目
		RUSH_ATK, //ロール攻撃
		CALL_PROJECTILE, //射撃物呼び出し
		DOUBLE_SPHERE_ATK, //ダブルスフィア攻撃
        SHOT_CUTTER,     //斬撃飛ばし
        RELOAD, //リロード
    };

    //エフェクトの種類
    enum class EFF_TYPE
    {
        BLAST,
    };

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
	/// @param _charaObj キャラクターオブジェクト
    /// @param _deck デッキ
    EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~EnemyCardAction(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 解放
    /// @param  
    void Release(void)override;

    /// @brief ジャンプ攻撃チャージ中取得
    /// @param  
    /// @return 
    const bool IsJumpAtkCharge(void)const override;

    /// @brief 敵のジャンプチャージ中のカード勝負数
    /// @param  
    /// @return 
    virtual const int GetJumpCardNum(void)const override { return jampCardNum_; }
    
private:

    //スタンプ攻撃
    static constexpr float STOMP_ATK_SHAKE_CNT = 1.0f;              //カメラシェイク時間
    static constexpr int STOMP_ATK_ROCK_NUM = 9;                    //岩の生成数

    //攻撃半径広がるスピード
    static constexpr float JUMP_ATK_COL_SPD = 5.0f;

    //広がる時間
    static constexpr float JUMP_ATK_CNT_MAX = 2.5f;

    //ジャンプチャージ時間
    static constexpr float JUMP_CHARGE_TIME = 6.0f;

    //ジャンプ高さ
    static constexpr float JUMP_HEIGHT = 100.0f;

    //ジャンプ攻撃アニメーション
    static constexpr float JUMP_ANIM_START = 16.4f;
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
    static constexpr float JUMP_ATK_START_RADIUS = 30.0f;						//ジャンプ攻撃の始まりの半径

    //エフェクトサイズ
	static constexpr float BLAST_EFF_SCL = 1.4f;            //爆発
	static constexpr float JUMP_CHARGE_EFF_SCL = 500.0f;      //ジャンプチャージ

    //攻撃カウント
    float atkCnt_;

    //ジャンプチャージカウント
    float jumpChargeCnt_;

    //ジャンプチャージ中のカード勝敗数カウント
    int jampCardNum_;

    //イージング
    std::unique_ptr<Easing>easing_;

    //状態遷移
    std::unordered_map<LogicBase::ENEMY_ATTACK_TYPE, std::function<void(void)>>changeCardAction_;

    //遷移
    void ChangeStomp(void);             //踏みつけ
    void ChangeJumpAtk(void);           //ジャンプ攻撃
    void ChangeReload(void);            //リロード

    //更新
    void UpdateStomp(void);             //踏みつけ      
    void UpdateJumpAtk(void);           //ジャンプ攻撃
    void UpdateReload(void);            //リロード

    //攻撃別の当たり判定情報
    std::map<ACT_TYPE, ATK_STATUS>atkTable_;

    //岩生成フラグ
    bool isGenerateRock_;
};