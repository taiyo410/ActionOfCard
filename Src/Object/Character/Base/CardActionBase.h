#pragma once
#include<queue>
#include<map>
#include "ActionBase.h"

class CardPresenter;
class CharacterBase;
class EffectController;

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

protected:

    //ジャンプ溜め中のカード出した回数
    static constexpr int JAMP_CHARGE_CARD_NUM_MAX = 3;

    //カードデッキ
    CardPresenter& cardPresent_;

    //オブジェクト
    CharacterBase& charaObj_;

    //カードアクション関数ポインタ
    std::function<void(void)> cardActFunc_;

    //カードアクション遷移
    std::map<CARD_ACT_TYPE, std::function<void(void)>>changeAction_;

    //攻撃アクションの文字列との対応
    std::unordered_map<CARD_ACT_TYPE, std::string>atkStatusStrTable_;

    //攻撃ステータステーブル
    std::map<CARD_ACT_TYPE, ATK_STATUS>atkStatusTable_;

    //アタックのQueue配列
    std::queue<std::function<void(void)>>cardFuncs_;

    //エフェクトコントローラ
    std::unique_ptr<EffectController>effect_;

    //コンボフラグ
    bool isCombo_;

    //デュエルモードでの待機中フラグ　true:待機中
    bool isDuelWait_;

    //攻撃座標
    VECTOR atkPos_;

    //状態遷移
    void ChangeCardAction(const CARD_ACT_TYPE& _type);

    //攻撃モーション
    void AttackMotion(const ATK_STATUS& _status, const Collider::TAG& _attackTag,const VECTOR& _localPos);

    /// @brief カードの勝敗を常に監視する。負けたらtrueを返し、のけぞり状態にする
    /// @param _attackTag 使っている当たり判定タグ
    /// @return 
    bool IsCardFailure(const Collider::TAG& _attackTag);

	//負けて攻撃が終了
	void FinishFailureAttack(const Collider::TAG _attackCol);

    //攻撃情報をセット
    void SetAtk(const ATK_STATUS& _actType);

    //コンボアクション遷移(あれば実装する)
    virtual void ChangeComboAction(void) {};

	//コンボ入力受付
	void ComboInput(void);

    /// @brief 攻撃アクションステータスを外部ファイルから取得
    /// @param _atk 格納する攻撃ステータス
    /// @param _dataName 取得したいデータ名
    void LoadAttackStatus(ATK_STATUS& _atk,const std::string _dataName);

    /// @brief 攻撃ステータスを個数分ロード
    /// @param  
    void LoadStatus(void);
};

