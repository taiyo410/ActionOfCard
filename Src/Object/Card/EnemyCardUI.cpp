#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/DataBank.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Card/CardSystem.h"
#include "../Card/CardUIController.h"
#include "EnemyCardUI.h"

EnemyCardUI::EnemyCardUI(void)
{
	charaType_ = JSON_ENEMY_STR;
}

EnemyCardUI::~EnemyCardUI(void)
{
}

void EnemyCardUI::Load(void)
{
	CardUIBase::Load();

	atkCardImg_ = resMng_.Load(ResourceManager::SRC::ENEMY_ATK_CARD_IMG).handleId_;

	//敵はリロードカードを使わないので読み込まない
	reloadCardImg_ = UtilityCommon::INITIAL_HANDLE;	

	resMng_.Load(ResourceManager::SRC::CARD_BREAK_SE);

	cardWinRes_ = ResourceManager::SRC::CARD_BREAK_SE;
}

void EnemyCardUI::Init(void)
{
	//カードデータの読み込み
	LoadCardData();

	//カード初期化
	InitCardUI();

	//状態遷移登録
	changeMoveState_ = {
	{CARD_SELECT::NONE, [this]() {ChangeNone(); } },
	{CARD_SELECT::DISITION, [this]() {ChangeDecision(); } },
	{CARD_SELECT::RELOAD_WAIT, [this]() {ChangeReloadWait(); } }
	};

	//状態の初期化
	ChangeSelectState(CARD_SELECT::NONE);

}

void EnemyCardUI::Update(void)
{
	//基底クラスの更新
	CardUIBase::Update();

	//常に勝敗を監視し、負けたら弾く
	ReactMoveCard(REACT_GOAL_CARD_POS);
}

void EnemyCardUI::Draw(void)
{
	//カード描画(共通)
	CardUIBase::Draw();

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG

}
#ifdef _DEBUG
void EnemyCardUI::DrawDebug(void)
{

}
#endif // _DEBUG

void EnemyCardUI::ChangeNone(void)
{
	cardUpdate_ = [this]() {UpdateNone(); };
}

void EnemyCardUI::ChangeDecision(void)
{
	//アクション配列に入れる
	actions_.emplace_back(*handCurrent_);

	//現在の選択カードを増やす
	handCurrent_++;

	//使用中状態に移行。カードUIを動かす
	for (auto& act : actions_)
	{
		act->ChangeDicisionEnemyCardMove();
		act->ChangeUsing();
	}

	cardUpdate_ = [this]() {UpdateDecision(); };
}

void EnemyCardUI::ChangeReloadWait(void)
{
	cardUpdate_ = [this]() {UpdateReloadWait(); };
}

void EnemyCardUI::UpdateNone(void)
{

}

void EnemyCardUI::UpdateDecision(void)
{
	//使用中カードを動かす
	DecisionMoveCardAll();

	//決定移動が終わったものを探す
	auto it = std::find_if(actions_.begin(), actions_.end(), [this](auto& act) {return act->GetDecisionCnt() > 0.0f; });

	//終わっていたらNONE状態へ移行
	if (it == actions_.end())
	{
		SetBasePosActionCards();
		ChangeSelectState(CARD_SELECT::NONE);
	}
}

void EnemyCardUI::UpdateReloadWait(void)
{
	InitCardUI();
	ChangeSelectState(CARD_SELECT::NONE);
}

void EnemyCardUI::UpdateDrawCardUI(void)
{
	for (auto& card : actions_)
	{
		card->Update();
	}
}

void EnemyCardUI::InitCardUI(void)
{
	//手札をすべて消去する
	handCards_.clear();

	//手札にすべての初期札を入れる
	for (auto& it : initialCards_)
	{
		it->ResetCount();
		handCards_.emplace_back(it);
	}

	//手札が空でなければ、選択中カードを手札の初めの配列へ設定する
	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}
