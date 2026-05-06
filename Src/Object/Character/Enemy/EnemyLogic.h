#pragma once
#include "../Base/LogicBase.h"
#include"../Object/Common/Transform.h"
class CharacterBase;

class EnemyLogic :
    public LogicBase
{

public:

	//CPUの思考ウェイト(高いほど出す頻度が高い)
	static constexpr int NORMAL_WEIGHT = 60;	//通常攻撃
	static constexpr int JUMP_WEIGHT = 30;		//ジャンプ攻撃


	//攻撃の重み
	struct ATTACK_WEIGHT {
		int stomp = NORMAL_WEIGHT;
		int jump = JUMP_WEIGHT;
	};

	/// @brief コンストラクタ
	/// @param _myTrans 自身の情報
	EnemyLogic(Transform& _myTrans);

	/// @brief デストラクタ
	/// @param  
	~EnemyLogic(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 攻撃種類によって硬直時間をセット
	/// @param  
	void SetFreezeCntByAttackType(void)override;

#ifdef _DEBUG
	void DebugUpdate(void);
	void DebugDraw(void)override;
#endif // _DEBUG

private:

	//硬直時間
	static constexpr float NORMAL_FREEZE_TIME = 3.0f;	//通常攻撃硬直時間
	static constexpr float JUMP_FREEZE_TIME = 0.7f;		//ジャンプ攻撃硬直時間

	//最初に急に行動しないためのフリーズ時間
	static constexpr float INIT_FREEZE_TIME = 1.5f;

	//カードクールタイム制限
	static constexpr float CARD_COOL_TIME = 1.0f;	

	//攻撃の射程
	static constexpr float ATK_RANGE = 200.0f;

	//プレイヤーまでの距離詰めのクールタイム
	static constexpr float MOVE_COOL_TIME = 0.5f;

	//プレイヤーに攻撃を仕掛けるプレイヤーとの距離
	static constexpr float ATK_DISTANCE = 250.0f;

	//シーンマネージャ
	SceneManager& scnMng_;

	//カード使用クールタイム
	float cardCoolCnt_;

	//攻撃の重み付け
	ATTACK_WEIGHT weight_;

	//硬直時間
	float freezeCnt_;

	//攻撃の届く範囲
	float atkRange_;
	
	//前回の攻撃種類
	ENEMY_ATTACK_TYPE prevAttackType_;

	//攻撃を決める
	void DecideAction(void);
};