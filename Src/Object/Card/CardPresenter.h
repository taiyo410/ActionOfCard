#pragma once
#include <memory>
#include "../Character/UIData/CharacterUIData.h"
#include "../Card/CardUiBase.h"

class UIManager;
class CardDeck; 

class CardPresenter
{

public:

	/// @brief コンストラクタ
	/// @param _type 種類
	/// @param _deck デッキ
	CardPresenter(CHARACTER_TYPE& _type,CardDeck& _deck);

	/// @brief デストラクタ
	/// @param  
	~CardPresenter(void);
	
	/// @brief カード使用済み
	/// @param  
	void FinishCard(void);

	/// @brief カード負け
	/// @param  
	void FailureCard(void);

	/// @brief カードを引く
	/// @param  
	void PutCard(void);

	/// @brief リボルバーの回転
	/// @param _moveLR 動かす方向
	void RoleRevolver(const CardUIBase::CARD_SELECT _moveLR);

	/// @brief カードの種類を取得
	/// @param  
	/// @return カードの種類
	const CardBase::CARD_TYPE GetCardType(void)const;

	/// @brief 手札の取得
	/// @param  
	/// @return 手札
	const std::vector<CardBase::CARD_TYPE> GetHandCardType(void)const;

	/// @brief デッキクラスでのリロード処理
	/// @param  
	void DeckReload(void);
	
	/// @brief カード変更
	/// @param  
	void ChangeCard(void);

	/// @brief 敵のリロード
	/// @param  
	void EnemyCardReload(void);

	/// @brief アクション遷移時にカードをアクション中のカードを開放
	/// @param  
	void ChangeAction(void);

	/// @brief カード負けしたかどうか
	/// @param  
	const bool IsCardFailure(void)const;

	/// @brief リロードカウント割合セット
	/// @param _per 割合
	void SetUIReloadCount(const float _per);

	/// @brief カードUIの状態変更
	/// @param _select 変更したい状態
	void ChangeUIState(const CardUIBase::CARD_SELECT _select);

	/// @brief カードUIの状態の取得
	/// @param  
	/// @return 
	const CardUIBase::CARD_SELECT GetCardUIState(void)const;
	
private:

	//カードデッキ
	CardDeck& deck_;

	//カードUI
	UIManager& uiMng_;

	//キャラクター
	CHARACTER_TYPE& type_;
};