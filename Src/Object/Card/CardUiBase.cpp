#include<iostream>
#include<fstream>
#include "../../Src/Lib/nlohmann/json.hpp"
#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Common/Easing.h"
#include "../Card/CardUIDraw.h"
#include "../Card/CardSystem.h"
#include "../Card/CardUIController.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "CardUIBase.h"

CardUIBase::CardUIBase(void):
selectState_(CARD_SELECT::RELOAD_WAIT),
atkCardImg_(-1),
reloadCardImg_(-1),
soundMng_(SoundManager::GetInstance()),
resMng_(ResourceManager::GetInstance()),
scnMng_(SceneManager::GetInstance()),
handCurrent_(),
centerPos_(),
cardNoImg_(nullptr),
reloadGauge_(),
numPos_(),
cardMoveCnt_(),
disitionCnt_(),
reloadPer_()
{
	cardTypeMap_ = {
		{"Attack", CardBase::CARD_TYPE::ATTACK}
	};
}

CardUIBase::~CardUIBase(void)
{
}

void CardUIBase::ChangeSelectState(const CARD_SELECT _select)
{
	if (selectState_ == _select)return;
	selectState_ = _select;
	changeMoveState_[selectState_]();
}

void CardUIBase::ChangeUsedActionCard(void)
{
	for (auto& act : actions_)
	{
		act->ChangeUsedCard();
	}

}

void CardUIBase::ChangeReactActionCard(void)
{
	PlayCardSound();
	for (auto& act : actions_)
	{
		act->ChangeReactCard();
	}
}

void CardUIBase::PlayCardSound(void)
{
	CardSystem::BATTLE_RESULT result = CardSystem::GetInstance().GetResult(0);
	ResourceManager::SRC winRes = ResourceManager::SRC::NONE;

	//負けた場合
	if (result == CardSystem::BATTLE_RESULT::BE_DRAW || result == CardSystem::BATTLE_RESULT::GIVE_DRAW
		|| result == CardSystem::BATTLE_RESULT::NONE || result == CardSystem::BATTLE_RESULT::FAILURE_USE_BE_REFLECTED)
	{
		winRes = ResourceManager::SRC::CARD_BE_REFLECTED_SE;
	}
	//勝った場合
	else if (result == CardSystem::BATTLE_RESULT::SUCCESS_CARD_BREAK)
	{
		winRes = ResourceManager::SRC::CARD_BREAK_SE;
	}

	//SEの重複再生を防ぐ
	if (!SoundManager::GetInstance().IsPlay(winRes))
	{
		SoundManager::GetInstance().Play(winRes, SoundManager::PLAYTYPE::BACK);
	}
}

void CardUIBase::LoadCardData(void)
{
	using json = nlohmann::json;
	
	//Jsonデータの取得
	json j = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;

	std::vector<CardBase::CARD_STATUS> cards;
	//キャラタイプからどのJsonデータを読み取るかを決める
	for (const auto& card : j[charaType_])
	{
		CardBase::CARD_STATUS status;
		//カードの強さの読み込み
		if (card.contains(CARD_POWER_PATH))
		{
			status.pow = card[CARD_POWER_PATH];
		}

		//カードの種類の読み込み
		if (card.contains(CARD_TYPE_PATH))
		{
			std::string typeStr = card[CARD_TYPE_PATH];
			status.type = cardTypeMap_[typeStr];
		}

		//配列の中に追加
		AddCardUi(status);
	}

	//リロードカードを末尾に追加
	AddCardUi(CardBase::CARD_STATUS{ RELOAD_CARD_POWER,CardBase::CARD_TYPE::RELOAD });
}

void CardUIBase::Load(void)
{
	cardNoImg_ = resMng_.Load(ResourceManager::SRC::NUMBERS_IMGS).handleIds_;
}

void CardUIBase::Init(void)
{
	
}

void CardUIBase::Update(void)
{
	//カード状態
	cardUpdate_();

	//見えている部分だけ更新
	UpdateDrawCardUI();

	//使用済みカードの大きさ補完
	UpdateUsedCard();
}

void CardUIBase::Draw(void)
{
	if (actions_.empty())return;
	for (auto& card : actions_)
	{
		card->Draw();
	}
}

void CardUIBase::AddCardUi(const CardBase::CARD_STATUS _status)
{
	int img = -1;

	//同じステータスを見つける
	auto it = cardImgs_.find(_status);

	//画像に番号を合わせる
	int num = _status.pow - 1;
	if (num == RELOAD_CARD_POWER) { num = MAX_CARD_POWER; }

	int drawNumImg = cardNoImg_[num];
	std::shared_ptr<CardUIController> info = std::make_shared<CardUIController>(drawNumImg);

	//カード画像がない場合は作成して配列に挿入
	if(it==cardImgs_.end())
	{
		img = MakeCardNumImg(_status);
		cardImgs_.emplace(_status, img);
	}
	else
	{
		//カード画像がある場合は配列から取得
		img = it->second;
	}
	info->SetCardImg(img);

	//カード情報セット
	info->SetCardImg(img);
	info->SetStatus(_status);
	info->Load();
	info->Init();

	//配列に挿入
	initialCards_.emplace_back(info);
}

void CardUIBase::DecisionMoveCardAll(void)
{
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		if (card->GetState() == CardUIController::CARD_STATE::REACT)continue;

		card->DecisionMove();
	}
}

void CardUIBase::UpdateUsedCard(void)
{
	//アクション中のカードがなければ飛ばす
	if (actions_.empty())return;

	//使用済みカードを消す
	for (auto& act : actions_)
	{
		act->EraseUsedCard();
	}

	//消去アニメーションが終わったカードはアクション配列から削除
	std::erase_if(actions_, [](auto& act) 
		{
			float cnt = act->GetSclCnt();
			return cnt < 0.0f;
		});
}

void CardUIBase::ReactMoveCard(const Vector2F& _goalPos)
{
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		card->ReactUpdate(_goalPos);
	}
}

void CardUIBase::AddHandCurrent(void)
{
	handCurrent_++;
	if (handCurrent_ == handCards_.end())
	{
		handCurrent_ = handCards_.begin();
	}
}

void CardUIBase::SubHandCurrent(void)
{
	if (handCurrent_ == handCards_.begin())
	{
		handCurrent_ = handCards_.end();
	}
	handCurrent_--;
}

int CardUIBase::MakeCardNumImg(const CardBase::CARD_STATUS& _status)
{
	//サイズ
	int img = UtilityCommon::INITIAL_HANDLE;

	//描画可能なスクリーンの作成
	img = MakeScreen(GRAPH_SIZE_X, GRAPH_SIZE_Y, true);

	//描画先を作成したスクリーンに変更
	SetDrawScreen(img);

	//作成したスクリーンで描画
	int typeImg = GetTypeImg(_status);
	DrawGraph(0, 0, typeImg, true);

	//中央座標取得
	Vector2F centerPos;
	GetGraphSizeF(typeImg, &centerPos.x, &centerPos.y);

	//数字画像の描画(リロードカードは除く)
	if (_status.type != CardBase::CARD_TYPE::RELOAD)
	{
		//番号サイズ取得
		Vector2F size = { 0.0f,0.0f };

		//カードの強さから番号画像を取得
		int num = _status.pow - 1;
		if (num < 0) { num = MAX_CARD_POWER; }
		GetGraphSizeF(cardNoImg_[num], &size.x, &size.y);

		//縮小倍率をかける
		size *= NUM_SCL;

		Vector2F numSizeHalf = size / 2.0f;
		Vector2F leftTopPos = NUM_LOCAL_POS - numSizeHalf;
		Vector2F rightBottomPos = NUM_LOCAL_POS + numSizeHalf;
		DrawExtendGraphF(leftTopPos.x, leftTopPos.y, rightBottomPos.x, rightBottomPos.y, cardNoImg_[num], true);
	}

	//描画先を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	return img;
}

const int CardUIBase::GetTypeImg(const CardBase::CARD_STATUS _status)
{
	int typeImg = -1;
	switch (_status.type)
	{
	case CardBase::CARD_TYPE::ATTACK:
		typeImg = atkCardImg_;
		break;
	case CardBase::CARD_TYPE::RELOAD:
		typeImg = reloadCardImg_;
	}
	return typeImg;
}

void CardUIBase::SetBasePosActionCards(void)
{
	for (auto& card : actions_)
	{
		card->SetBaseCardPos();
	}
}

