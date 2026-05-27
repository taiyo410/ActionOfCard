#pragma once

#include "../Base/CardActionBase.h"

class PlayerCardAttackAction :
    public CardActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
    /// @param _charaObj キャラクターオブジェクト
    /// @param _deck デッキ
    PlayerCardAttackAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardAttackAction(void)override;

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

    /// @brief アクションデータのロード
    /// @param animVar 
    void LoadAnimVar(const ACTION_LOAD_DATA& animVar) override;

    ////カードアクション遷移
    //void ChangeCardAction(const CARD_ACTION_TYPE _type);

    //遷移系
    void ChangeShortAttackOne(void);            //攻撃アクション1回目(近距離)
    void ChangeMiddleAttackOne(void);           //攻撃アクション1回目(中距離)
    void ChangeAttackTwo(void);                 //攻撃アクション2回目
    void ChangeAttackThree(void);               //攻撃アクション3回目
    //void ChangeMagicFire(void);                 //炎魔法
    void ChangeReload(void);                    //リロード

private:

    //カプセル球の半径(プレイヤーは武器のコライダを持っているため不要)
    static constexpr float ATK_SPHERE_RADIUS = 0.0f;

    //攻撃1段目判定(中距離)
    static constexpr float ATTACK_ONE_MID_COMBO_TIME = 0.2f;            //コンボ受付時間

    //中距離攻撃
    static constexpr float ATTACK_ONE_MID_TIME = 1.0f;                  //中距離移動時間
    static constexpr float ATTACK_ONE_MID_SPD = 30.0f;                  //中距離初期速度

    //攻撃3段目判定
    static constexpr float ATTACK_THREE_ANIM_SPD = 10.0f;               //攻撃アニメーション3段階目の溜め時のアニメ速度
    static constexpr float ATTACK_THREE_ANIM_LERP_TIME = 0.4f;          //攻撃3段階目アニメスピード補完時間

    //リロード
    static constexpr float RELOD_EFF_SCL = 100.0f;                      //エフェクトスケール
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

    ////カードアクション
    //CARD_ACTION_TYPE cardActType_;

    ////カードアクション遷移
    //std::unordered_map<CARD_ACTION_TYPE, std::function<void(void)>> changeCardAction_;

    ////攻撃ステータステーブル
    //std::unordered_map<CARD_ACTION_TYPE, ATK_STATUS> atkStatusTable_;

    ////攻撃アクション文字列
    //std::unordered_map<std::string, CARD_ACTION_TYPE> attackActionStr_;

    //攻撃アクションごとのアニメーション
    std::unordered_map<CARD_ACTION_TYPE, AnimationController::ANIMATION_VARIABLE>atkAnimVals_;

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

    //リロードカウントのセット
    void SetUIReloadCnt(void);

    //攻撃状態遷移時のカード初期化
    void ChangeActionCardInit(void);

    //更新系
    void UpdateAttack(void);                    //攻撃アクション
    void UpdateMiddleAttack(void);              //中距離攻撃アクション１回目(中距離)
    void UpdateAttackThree(void);               //攻撃アクション３段階目(１，２段目とは違う演出を入れる)
    //void UpdateFireMagic(void);                 //炎魔法
    void UpdateReload(void);                    //リロード



    //コンボアクション遷移(あれば実装する)
    void ChangeComboAction(void)override;

};

