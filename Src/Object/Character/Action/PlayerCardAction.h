#pragma once
#include<functional>
#include<queue>
#include "../Base/CardActionBase.h"

class Easing;
class CardDeck;
class CharacterBase;

class PlayerCardAction :
    public CardActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
	/// @param _charaObj キャラクターオブジェクト
    /// @param _deck デッキ
    PlayerCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardAction(void)override;

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

	/// @brief リロードエフェクトの解放
	/// @param  
	void ReleaseReloadResource(void)override;

private:

    //カプセル球の半径(プレイヤーは武器のコライダを持っているため不要)
    static constexpr float ATK_SPHERE_RADIUS = 0.0f;				    

    //攻撃1段目判定(中距離)
    static constexpr float ATTACK_ONE_MID_ANIM_START = 13.0f;           //アニメーションスタート位置
    static constexpr float ATTACK_ONE_MID_ANIM_END = 37.0f;             //アニメーション終了位置
    static constexpr float ATTACK_ONE_MID_COMBO_TIME = 0.2f;            //コンボ受付時間

    //中距離攻撃
    static constexpr float ATTACK_ONE_MID_TIME = 1.0f;                  //中距離移動時間
    static constexpr float ATTACK_ONE_MID_SPD = 30.0f;                  //中距離初期速度

    //攻撃3段目判定
    static constexpr float ATTACK_THREE_ANIM_START = 60.0f;             //攻撃３段階目アニメーションスタート
    static constexpr float ATTACK_THREE_ANIM_GOAL = 86.0f;              //攻撃３段階目アニメーションゴール
    static constexpr float ATTACK_THREE_ANIM_SPD = 10.0f;               //攻撃アニメーション3段階目の溜め時のアニメ速度
    static constexpr float ATTACK_THREE_ANIM_LERP_TIME = 0.4f;          //攻撃3段階目アニメスピード補完時間

    //リロード
    static constexpr float RELOD_EFF_SCL = 100.0f;                      //エフェクトスケール
    static constexpr float RELOAD_START_STEP = 17.0f;                   //アニメーション始まりステップ
    static constexpr float RELOAD_END_STEP = 33.0f;                     //アニメーション終わり
    static constexpr float RELOAD_LOOP_START = 26.0f;                   //アニメーションループスタート
    static constexpr float RELOAD_LOOP_END = 30.0f;                     //アニメーションループ終了

    //攻撃段階
    static constexpr int ATTACK_ONE = 0;
    static constexpr int ATTACK_TWO = 1;
    static constexpr int ATTACK_THREE = 2;

    //リロードのボタン押す時間
    static constexpr float RELOAD_TIME = 3.0f;

    //攻撃ローカル座標
    static constexpr VECTOR ATK_ONE_LOCAL = { 0.0f,0.0f,50.0f };

    //カードリロード中の音量
    static constexpr float CARD_RELOAD_VOL = 0.6f;

    //攻撃の当たり判定始まりカウント
    float attackStartAnimcnt_;

    //攻撃の当たり判定終わりカウント
    float attackEndAnimCnt_;

    //攻撃段階
    int attackStageNum_;

    //リロードカウント
    float pushReloadCnt_;

    //中距離攻撃突きカウント
    float midAtkCnt_;
    float midAtkOverCnt_;

    //攻撃3段階目判定終了カウント
    float atkThreeEndCnt_;

    //攻撃3段階目のアニメーション速度補完
    float atkAnimLerpCnt_;

    //イージング
    std::unique_ptr<Easing>easing_;

    //攻撃条件
    bool IsAttackable(void);

    //連続攻撃条件
    bool IsCanComboAttack(void);

    //１段目を中距離攻撃アクションか近距離攻撃アクションかを決める
    void DecideAttackOne(void);

    //攻撃状態遷移時のカード初期化
    void ChangeActionCardInit(void);

    //リロードカウントのセット
	void SetUIReloadCnt(void);

    //攻撃のステータスロード
    void LoadAttack(void);

    //更新系
	void UpdateAttack(void);                   //攻撃アクション
    void UpdateMiddleAttack(void);             //中距離攻撃アクション１回目(中距離)
    void UpdateAttackThree(void);              //攻撃アクション３段階目(１，２段目とは違う演出を入れる)
    void UpdateReload(void);                   //リロード

    //遷移系
	void ChangeShortAttackOne(void);           //攻撃アクション1回目(近距離)
	void ChangeMiddleAttackOne(void);          //攻撃アクション1回目(中距離)
	void ChangeAttackTwo(void);                //攻撃アクション2回目
	void ChangeAttackThree(void);              //攻撃アクション3回目
    void ChangeReload(void);                   //リロード

    //コンボアクション遷移(あれば実装する)
    void ChangeComboAction(void)override;
};

