#pragma once
#include<DxLib.h>
#include "../Object/Common/Transform.h"

class CharacterBase;

class LogicBase
{

public:

    //アクションボタンの種類
    enum class ACT_CNTL
    {
        NONE            //何もしていないとき
        , MOVE          //移動
        , DASHMOVE      //ダッシュ
        , PUNCH         //パンチ
        , JUMP          //ジャンプ
        , CARD_CHARGE   //カードチャージ
        , CARD_USE      //カード使用
        , CARD_MOVE_LEFT //カード左
        , CARD_MOVE_RIGHT//カード右
    };

    //敵の攻撃タイプ
    enum class ENEMY_ATTACK_TYPE {
        NONE,
        STOMP,
        JUMP,
        ROAR,
        ROLE
    };

    //どの操作がされたか
    struct ACT_FLAG
    {
        //カード関連
        bool isCardMoveLeft = false;    //カードを左へ
        bool isCardMoveRight = false;   //カードを右へ
        bool isCardUse = false;         //カード使用
        bool isCardCharge = false;      //カードをチャージ配列へ  
        bool isCardPushKeep = false;    //カード使用キー長押し

        //アクション関連
		bool isRun = false;         //走る
		bool isJump = false;        //ジャンプ
		bool isDodge = false;       //回避
    };



	/// @brief コンストラクタ
    /// @param  
    LogicBase(Transform& _myTrans);
	
    /// @brief デストラクタ
    /// @param  
    virtual ~LogicBase(void);
    
    /// @brief 初期化
    /// @param  
    virtual void Init(void) = 0;
    
    /// @brief 更新
    /// @param  
    virtual void Update(void) = 0;
    
    /// @brief 入力方向の取得
    /// @param  
    /// @return 
    VECTOR GetInputDir(void)const;

    /// @brief 移動方向の取得
    /// @param  
    /// @return 方向
    const VECTOR& GetDir(void)const { return moveDir_;}
    
    /// @brief アクション入力の取得
    /// @param  
    /// @return アクション入力
    const ACT_FLAG GetIsAct(void)const { return isAct_; }

    /// @brief ランダムで決まった攻撃の種類を取得
    /// @param  
    /// @return 攻撃種類
    const ENEMY_ATTACK_TYPE& GetAttackType(void)const { return attackType_; }

    /// @brief ターゲットをセットする
    /// @param _target 
    void SetTargetCharacter(std::shared_ptr<CharacterBase>_target);

	/// @brief 方向をモデルの前方向にセット
	/// @param _trans モデル情報 
    void SetMoveDir(const VECTOR _dir) { moveDir_ = _dir;}

    /// @brief ターゲットの方向を見る
    /// @param  
    void GetLookAtTargetDir(void);

    /// @brief ターゲットと当たった
    /// @param  
    /// @return true:当たっている:false:当たっていない
    const bool HitTarget(void)const;

    //相手の座標情報を取得
    const Transform GetTargetTransform(void);

    /// @brief 敵のジャンプチャージ中のカード勝負数
    /// @param  
    /// @return 
    const int GetJumpCardNum(void)const;

    /// @brief 方向を決める
    /// @param _deg デグリー角度
    /// @param _dir ベクトル
    void SetDir(const VECTOR _dir){moveDir_ = _dir;}

    /// @brief 方向入力がされているか
    /// @param  
    /// @return true:されている　false:されていない
    const bool GetIsEnteredDir(void);

	/// @brief アクション中フラグセット
	/// @param  
	void IsActioningSet(void) { isActioning_ = false; }

    /// @brief 攻撃種類によって硬直時間をセット
    /// @param  
    virtual void SetFreezeCntByAttackType(void) {};

    /// @brief 相手のキャラクターの情報の取得
    /// @param  
    /// @return 
    const std::weak_ptr<CharacterBase>GetTargetCharacter(void)const { return targetChara_; }

    /// @brief ターゲットと自分の距離の取得
    /// @param  
    /// @return 
    const float GetTargetDis(void)const;

#ifdef _DEBUG
    virtual void DebugDraw(void) {};
#endif // _DEBUG

protected:

	//操作管理用
	ACT_CNTL actCntl_;

    //移動方向ベクトル(これをもとにキャラクターが動く)
	VECTOR moveDir_;            

    //入力方向(入力していないときは{0,0,0})
    VECTOR inputDir_;           

    //前回の移動方向ベクトル
	VECTOR prevMoveDir_;        

    //攻撃種類
    ENEMY_ATTACK_TYPE attackType_;
    
    //ターゲット
    std::weak_ptr<CharacterBase> targetChara_;

    //アクション入力がされたかどうか
    ACT_FLAG isAct_;

    //アクション中
	bool isActioning_;

    //自身のモデル情報
    Transform& myTrans_;
};

