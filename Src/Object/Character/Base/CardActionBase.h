#pragma once
#include<queue>
#include<map>
#include "Object/Common/Collider.h"
#include "./ActionBase.h"

class CardPresenter;
class CharacterBase;

class CardActionBase :
    public ActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
    /// @param _charaObj キャラクター
    /// @param _deck デッキ
    CardActionBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

    /// @brief デストラクタ
    /// @param  
    ~CardActionBase(void)override;

    /// @brief 初期化
    /// @param  
    virtual void Init(void) override=0;

    /// @brief 更新
    /// @param  
    virtual void Update(void) override = 0;

    /// @brief 敵のジャンプチャージ中のカード勝負数
    /// @param  
    /// @return 
    virtual const int GetJumpCardNum(void)const override { return -1; }

    /// @brief リロードエフェクトの解放
    /// @param  
    virtual void ReleaseReloadResource(void);

    /// @brief カードの勝敗を常に監視する。負けたらtrueを返し、のけぞり状態にする
    /// @param _attackTag 使っている当たり判定タグ
    /// @return 
    bool IsCardFailure(const Collider::TAG& _attackTag);

protected:

#pragma region メンバー変数
    CardPresenter& cardPresent_;    //カードデッキ
    //カードアクション関数ポインタ
    std::function<void(void)> cardActFunc_;
    //アタックのQueue配列
    std::queue<std::function<void(void)>>cardFuncs_;
    bool isCombo_;          //コンボフラグ
    bool isDuelWait_;       //デュエルモードでの待機中フラグ　true:待機中
    VECTOR atkPos_;         //攻撃座標
    int atkAnim_;           //攻撃アニメーション
#pragma endregion

#pragma region メンバー関数
    //攻撃のステータスロード
    void LoadAttackStatus(const nlohmann::json& _jsonData, ATK_STATUS& _atkStatus);

    //攻撃モーション
    void AttackMotion(const ATK_STATUS& _status, const Collider::TAG& _attackTag, const VECTOR& _localPos);

    //負けて攻撃が終了
    void FinishFailureAttack(const Collider::TAG _attackCol);

    //攻撃情報をセット
    void SetAtk(const ATK_STATUS& _actType);

    //コンボアクション遷移(あれば実装する)
    virtual void ChangeComboAction(void) {};

    //コンボ入力受付
    void ComboInput(void);
#pragma endregion
};

