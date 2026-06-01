#pragma once

#include "../../Base/CardActionBase.h"

class PlayerCardAttackBase : 
	public CardActionBase
{

public:

	/// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
	/// @param _charaObj キャラクターオブジェクト
	/// @param _deckPresent カードプレゼンター
	PlayerCardAttackBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deckPresent);

	/// @brief デストラクタ
	/// @param  
	~PlayerCardAttackBase(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 攻撃別の初期化
	/// @param  
	virtual void InitAttack(void) {};

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 解放
	/// @param  
	void Release(void) override;



	/// @brief 攻撃アクションの内容
	/// @param  
	virtual void AttackUpdate(void) {}

	/// @brief アクション毎の解放
	/// @param  
	virtual void AttackRelease(void){}

private:

};

