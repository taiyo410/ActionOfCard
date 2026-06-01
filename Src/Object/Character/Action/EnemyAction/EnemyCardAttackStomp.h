#pragma once
#include "EnemyCardAttackBase.h"
class EnemyCardAttackStomp :
    public EnemyCardAttackBase
{

public:

	/// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
	/// @param _charaObj キャラクターオブジェクト
	/// @param _cardPresent カードプレゼンター
	EnemyCardAttackStomp(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent);

	/// @brief デストラクタ
	/// @param  
	~EnemyCardAttackStomp(void)override;

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
	/// @param _data 読み込むデータ
	void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

	//スタンプ攻撃
	static constexpr float STOMP_ATK_SHAKE_CNT = 1.0f;              //カメラシェイク時間
	static constexpr int STOMP_ATK_ROCK_NUM = 9;                    //岩の生成数

	//岩生成フラグ
	bool isGenerateRock_;
};

