#include <DxLib.h>
#include <cmath>
#include <ranges>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include"../Manager/Generic/DataBank.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Card/CardUIController.h"
#include "../Card/CardSystem.h"
#include "PlayerCardUI.h"

PlayerCardUI::PlayerCardUI(void):
radius_({RADIUS_X,RADIUS_Y}),
isReloadEnd_(false),
reloadAnimCurr_(),
cardNumFrameImg_(UtilityCommon::INITIAL_HANDLE),
cardNumMaskImg_(UtilityCommon::INITIAL_HANDLE),
cardNumBgImg_(UtilityCommon::INITIAL_HANDLE),
fontHandle_(UtilityCommon::INITIAL_HANDLE),
reloadFontHandle_(UtilityCommon::INITIAL_HANDLE),
imgRevolverArrow_(UtilityCommon::INITIAL_HANDLE),
cardNumPer_(UtilityCommon::RATIO_MAX)
{
	charaType_ = JSON_PLAYER_STR;
}

PlayerCardUI::~PlayerCardUI(void)
{
	handCards_.clear();
	visibleCards_.clear();
	actions_.clear();
	changeMoveState_.clear();
	initialCards_.clear();
}

void PlayerCardUI::Load(void)
{
	CardUIBase::Load();

	//リソースのロード
	atkCardImg_ = resMng_.Load(ResourceManager::SRC::PLAYER_ATK_CARD_IMG).handleId_;
	reloadCardImg_ = resMng_.Load(ResourceManager::SRC::RELOAD_CARD_IMG).handleId_;
	reloadGauge_ = resMng_.Load(ResourceManager::SRC::RELOAD_GAUGE).handleId_;
	cardNumFrameImg_ = resMng_.Load(ResourceManager::SRC::P_CARD_NUM_GAUGE_FRAME).handleId_;
	cardNumMaskImg_ = resMng_.Load(ResourceManager::SRC::P_CARD_NUM_GAUGE_MASK).handleId_;
	fontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE,0);
	reloadFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), RELOAD_FONT_SIZE,0);
	cardNumBgImg_ = resMng_.Load(ResourceManager::SRC::P_CARD_NUM_GAUGE_BACK).handleId_;
	resMng_.Load(ResourceManager::SRC::CARD_MOVE_SE);
	resMng_.Load(ResourceManager::SRC::CARD_BE_REFLECTED_SE);
	resMng_.Load(ResourceManager::SRC::CARD_PUT_SE);
	cardWinRes_ = ResourceManager::SRC::CARD_BREAK_SE;

	imgRevolverArrow_ = resMng_.Load(ResourceManager::SRC::CARD_REVOLVER_L_ARROW).handleId_;

}
void PlayerCardUI::Init(void)
{
	cardGaugePSMaterial_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::HPBAR_PS);
	cardGaugePSRenderer_ = std::make_unique<PixelRenderer>(*cardGaugePSMaterial_);

	cardGaugePSMaterial_->AddTextureBuf(cardNumMaskImg_);
	changeMoveState_ = {
		{CARD_SELECT::NONE, [this]() {ChangeNone(); } },
		{CARD_SELECT::LEFT, [this]() {ChangeLeft(); } },
		{CARD_SELECT::RIGHT, [this]() {ChangeRight(); } },
		{CARD_SELECT::DISITION, [this]() {ChangeDecision(); } },
		{CARD_SELECT::RELOAD_WAIT, [this]() {ChangeReloadWait(); } },
		{CARD_SELECT::RELOAD, [this]() {ChangeReload(); } }
	};

	//マテリアル関連
	cardGaugePSMaterial_->AddConstBuf(BAR_LIGHT_GREEN);
	cardGaugePSMaterial_->AddConstBuf(BAR_BLUE);
	cardGaugePSMaterial_->AddConstBuf({ cardNumPer_,cardNumPer_,0.0f,0.0f });

	//頂点作成
	cardGaugePSRenderer_->MakeSquareVertex(BAR_POS, BAR_SIZE);

	//Jsonからカードデータを読み込む
	LoadCardData();

	//カードの初期化
	InitCardUI();

	//状態の初期化
	ChangeSelectState(CARD_SELECT::NONE);

	//見せカードの座標をセット
	SetBasePosVisibleCards();

}

void PlayerCardUI::Update(void)
{
	CardUIBase::Update();

	//常に見せカードは上下にふわふわ動かす
	MoveUpDownVisibleCards();

	//定数バッファのセット
	float initCardNum = static_cast<float>(initialCards_.size());
	float handCardNum = static_cast<float>(handCards_.size());
	cardNumPer_ = handCardNum / initCardNum;
	cardGaugePSMaterial_->SetConstBuf(CARD_NUM_GAUGE_CONST_BUF_IDX, { cardNumPer_,cardNumPer_,0.0f,0.0f });

	//弾かれるカードの大きさ補完
	ReactMoveCard(REACT_GOAL_CARD_POS);
}

void PlayerCardUI::Draw(void)
{
	//逆順で描画
	for (auto& card : visibleCards_ | std::ranges::views::reverse)
	{
		//選択中のカードはスキップ
		if (GetSearchHandIt(card)==handCurrent_)
		{
			continue;
		}
		card->Draw();
		if (card->GetStatus().type == CardBase::CARD_TYPE::RELOAD)
		{
			card->DrawReloadGauge(reloadPer_);
			Vector2F pos = card->GetCenterPos();
			UtilityDraw::DrawStringCenter(static_cast<int>(pos.x), static_cast<int>(pos.y - RELOAD_STR_OFF_Y), RELOAD_STR, UtilityCommon::WHITE, reloadFontHandle_);
		}
	}

	if (handCurrent_ != handCards_.end())
	{
		(*handCurrent_)->DrawSelectCard();
		//リロードカードの描画
		if ((*handCurrent_)->GetStatus().type == CardBase::CARD_TYPE::RELOAD)
		{
			(*handCurrent_)->DrawReloadGauge(reloadPer_);
			Vector2F pos = (*handCurrent_)->GetCenterPos();
			UtilityDraw::DrawStringCenter(static_cast<int>(pos.x), static_cast<int>(pos.y - RELOAD_STR_OFF_Y), RELOAD_STR, UtilityCommon::WHITE, reloadFontHandle_);
		}
		//選択カード枠描画
		(*handCurrent_)->SelectCardDrawFrame();
	}

	//カード残り枚数ゲージ背景の描画
	DrawExtendGraphF(BAR_BG_POS.x, BAR_BG_POS.y, BAR_BG_POS.x + BAR_BG_SIZE.x, BAR_BG_POS.y + BAR_BG_SIZE.y, cardNumBgImg_, true);

	//カード残り枚数ゲージの描画
	cardGaugePSRenderer_->Draw();

	//カード枚数ゲージのフレーム
	DrawExtendGraphF(BAR_POS.x, BAR_POS.y, BAR_POS.x + BAR_SIZE.x, BAR_POS.y + BAR_SIZE.y, cardNumFrameImg_, true);

	int handCardSize = static_cast<int>(handCards_.size());

	//カードの残り枚数の描画
	DrawFormatStringFToHandle(
		FONT_POS.x,
		FONT_POS.y,
		UtilityCommon::RED,
		fontHandle_,
		L"%d",
		handCardSize
	);

	//カード描画(共通)
	CardUIBase::Draw();
	
	//矢印とボタン描画
	DrawArrowAndBotton();

#ifdef _DEBUG
	//DrawDebug();
#endif // _DEBUG

}

#ifdef _DEBUG
void PlayerCardUI::DrawDebug(void)
{

}
#endif 
void PlayerCardUI::InitCardUI(void)
{
	//初めに配列を初期化
	handCards_.clear();
	visibleCards_.clear();
	actions_.clear();

	//手札にすべての初期札を入れる
	for (auto& it : initialCards_)
	{
		(*it).Load();
		(*it).Init();
		handCards_.emplace_back(it);
	}

	//始めの配列にリロードカードを描画したいので、最後の配列にセットする
	auto beginit = std::prev(handCards_.end());
	auto endIt = handCards_.begin();

	//endItをbeginの５個先(６枚目)に指定する
	std::advance(endIt, VISIBLE_CARD_MAX - 1);

	//座標を指定する時に使用する
	int i = 0;
	//６枚目までイテレータを回す
	for (auto& it = beginit; it != endIt; it++)
	{
		//手札イテレータが終端なら先頭に戻す
		if(it==handCards_.end())
		{
			it = handCards_.begin();
		}

		//カードの初期化
		(*it)->InitCard(i);

		//見せるカード配列に入れる
		visibleCards_.emplace_back(*it);
		i++;
	}

	//現在のカードをセット
	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}

// _DEBUG
void PlayerCardUI::ChangeNone(void)
{
	//イージング時間のセット
	cardMoveCnt_ = CardUIController::SELECT_MOVE_CARD_TIME;

	//目標角度を現在の角度にする
	for (auto& card : visibleCards_)
	{
		//card->SyncCardAngleAndPos();
		card->SetStartAndGoalAngle(0.0f);
	}

	cardUpdate_ = [this]() {UpdateNone(); };
}

void PlayerCardUI::ChangeLeft(void)
{
	// 時計回りにカードを回す
	// 対象は、手札UIの全てのカード
	cardMoveCnt_ = CardUIController::SELECT_MOVE_CARD_TIME;

	//カードの範囲変数を更新する
	// 手札はリロードカードが必ず残るので、最小枚数は1枚(0はあり得ないようにする)
	if (visibleCards_.size() == 1)
	{
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}

	//先頭に追加
	auto it = handCurrent_; // 手札の現在選択カード(handCards_のイテレータ
	const int rupeNum = static_cast<int>(visibleCards_.size()) - 1;
	for (int i = 0; i < rupeNum;i++)
	{
		it++;
		if (it == handCards_.end())
		{
			it = handCards_.begin();
		}
	}

	//次の角度を現在角度に代入
	//見せるカードのマックス分角度をかける
	int size = static_cast<int>(visibleCards_.size());
	(*it)->SetCurrentAngle(ARROUND_PER_RAD * (size - CARDS_BEFORE_CURRENT));

	//見せカードの末尾に追加
	visibleCards_.emplace_back(*it);

	//手札選択カードを更新
	AddHandCurrent();

	//イージングするために始点と終点をセット
	for (auto& card : visibleCards_)
	{
		float currentAngle = card->GetCurrentAngle();
		card->SetStartAndGoalAngle(currentAngle - ARROUND_PER_RAD);
	}
	//サウンドを再生
	soundMng_.Play(ResourceManager::SRC::CARD_MOVE_SE, SoundManager::PLAYTYPE::BACK);

	cardUpdate_ = [this]() {UpdateLeft(); };
}

void PlayerCardUI::ChangeRight(void)
{
	//イージング補完時間をセット
	cardMoveCnt_ = CardUIController::SELECT_MOVE_CARD_TIME;

	//visible配列に入れる前に現在の番地を引くことで、
	//結果的に1番目が選択されていることになる
	if(visibleCards_.size()==1)
	{
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	
	//先頭に追加
	auto it = handCurrent_;

	//現在位置より２枚遡って配列に入れる
	for (int i = 0; i < PREV_CARD_COUNT; i++)
	{
		if (it == handCards_.begin())
		{
			it = handCards_.end();
		}
		it--;
	}

	//角度をセット
	(*it)->SetCurrentAngle(-ARROUND_PER_RAD * PREV_CARD_COUNT);

	//見せカードの先頭に追加
	visibleCards_.emplace_front(*it);

	//手札選択カードを更新
	SubHandCurrent();

	//目標角度をずらす
	for (auto& card : visibleCards_)
	{
		float currentAngle = card->GetCurrentAngle();
		card->SetStartAndGoalAngle(currentAngle + ARROUND_PER_RAD);
	}

	//サウンドを再生
	soundMng_.Play(ResourceManager::SRC::CARD_MOVE_SE, SoundManager::PLAYTYPE::BACK);
	
	cardUpdate_ = [this]() {UpdateRight(); };
}

void PlayerCardUI::ChangeDecision(void)
{
	// カードを使う処理
	if (selectState_ == CARD_SELECT::NONE&&(*handCurrent_)->GetStatus().type == CardBase::CARD_TYPE::RELOAD)
	{
		ChangeSelectState(CARD_SELECT::RELOAD_WAIT);
		return;
	}
	actions_.emplace_back(*handCurrent_);

	//決定カウントをセット
	for (auto& act : actions_)
	{
		//弾かれ状態の場合は飛ばす
		if (act->GetState() == CardUIController::CARD_STATE::REACT)continue;

		//使用中状態にする
		act->ChangeUsing();

		//決定した時のイージング時間セット
		act->SetDecisionCount(CardUIController::DISITION_MOVE_CARD_TIME);
	}
	
	//手札に6枚よりカードが多かったら配列に入れる
	UpdateVisibleCard();

	//手札から使用するカードを消去
	EraseHandCard();

	//カードの範囲変数を更新する
	DecideGoalAngle();

	cardUpdate_ = [this]() {UpdateDecision(); };
}

void PlayerCardUI::ChangeReloadWait(void)
{
	cardMoveCnt_ = 0.0f;
	cardUpdate_ = [this]() {UpdateReloadWait(); };
}
void PlayerCardUI::ChangeReload(void)
{
	//配列の初期化
	handCards_.clear();
	visibleCards_.clear();

	//リロード終了フラグの初期化
	isReloadEnd_ = false;

	//一番最後の配列を見る
	reloadAnimCurr_ = std::prev(initialCards_.end());

	//リロードアニメーション中はカレントを終端にする
	handCurrent_ = handCards_.end();
	cardUpdate_ = [this]() {UpdateReload(); };
}
void PlayerCardUI::UpdateNone(void)
{

}

void PlayerCardUI::UpdateLeft(void)
{
	//イージングカウント
	cardMoveCnt_ -= DELTA;

	//イージングが終わったら状態を戻す
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_front();
		SetBasePosVisibleCards();
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}

	//カードの移動
	MoveCardAll(CardUIController::SELECT_MOVE_CARD_TIME);
}

void PlayerCardUI::UpdateRight(void)
{
	//イージングカウント
	cardMoveCnt_ -= DELTA;

	//移動が終わったら状態を戻す
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_back();
		SetBasePosVisibleCards();
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}

	//カードの移動
	MoveCardAll(CardUIController::SELECT_MOVE_CARD_TIME);
}

void PlayerCardUI::UpdateDecision(void)
{
	DecisionMoveCardAll();

	std::list<std::shared_ptr<CardUIController>>::iterator visibleCurrent = GetVisibleCurrentIt();
	for (auto it = visibleCurrent; it != visibleCards_.end(); it++)
	{
		(*it)->MoveOnRevolver(cardMoveCnt_,CardUIController::DISITION_MOVE_CARD_TIME);
	}
	cardMoveCnt_ -= DELTA;

	//決定移動が終わったカードを探す
	auto it = std::find_if(actions_.begin(), actions_.end(), [this](auto& act) {return act->GetDecisionCnt() > 0.0f; });

	//終わっていたら、NONE状態へ
	if(it==actions_.end())
	{
		SetBasePosVisibleCards();
		SetBasePosActionCards();
		ChangeSelectState(CARD_SELECT::NONE);
	}
}

void PlayerCardUI::UpdateReloadWait(void)
{
	if(reloadPer_>= UtilityCommon::RATIO_MAX)
	{
		//一瞬none状態にし、必ずRELOADの遷移処理をさせる
		ChangeSelectState(CARD_SELECT::NONE);
		
		ChangeSelectState(CARD_SELECT::RELOAD);

		reloadPer_ = 0.0f;
		return;
	}
}
void PlayerCardUI::UpdateReload(void)
{
	//リロードアニメーションの更新
	ReloadAnimation();

	//リロードが終了したら
	if (isReloadEnd_)
	{
		//見せるカードの現在位置イテレータを初期化
		if (cardMoveCnt_ >= 0.0f)
		{
			cardMoveCnt_ -= DELTA;
			MoveCardAll(CardUIController::RELOAD_MOVE_CARD_TIME_PER);
		}
		else
		{
			//選択カードを先頭にセット
			if (!handCards_.empty())
			{
				handCurrent_ = handCards_.begin();
			}
			SetBasePosVisibleCards();
			ChangeSelectState(CARD_SELECT::NONE);
		}
	}
}

void PlayerCardUI::MoveCardAll(const float& _moveTImeMax)
{
	for (auto& card : visibleCards_)
	{
		card->MoveOnRevolver(cardMoveCnt_, _moveTImeMax);
	}
}

void PlayerCardUI::UpdateDrawCardUI(void)
{
	//見えている部分だけ更新
	for (auto& card : visibleCards_)
	{
		card->Update();
	}
}


void PlayerCardUI::SetBasePosVisibleCards(void)
{
	for (auto& card : visibleCards_)
	{
		card->SetBaseCardPos();
		card->SetStartAngle();
	}
}

void PlayerCardUI::UpdateVisibleCard(void)
{
	const int size = static_cast<int>(handCards_.size());
	//手札に6枚よりカードが多かったら配列に入れる
	if (size > VISIBLE_CARD_MAX)
	{
		//先頭に追加
		auto endIt = handCurrent_;
		auto visibleIt = GetVisibleCurrentIt();

		//表示カードの次の配列になるまで手札のイテレータを回す
		for (; visibleIt != visibleCards_.end(); visibleIt++)
		{
			endIt++;
			if (endIt == handCards_.end())
			{
				endIt = handCards_.begin();
			}
		}
		(*endIt)->SetCurrentAngle(ARROUND_PER_QUAD_RAD + ARROUND_PER_RAD);
		visibleCards_.emplace_back(*endIt);
	}
}


void PlayerCardUI::EraseHandCard(void)
{
	std::list<std::shared_ptr<CardUIController>>::iterator eraseHandIt = handCurrent_;
	std::list<std::shared_ptr<CardUIController>>::iterator eraseVisibleIt = GetVisibleCurrentIt();
	std::list<std::shared_ptr<CardUIController>>::iterator visibleCurrentIt = eraseVisibleIt;

	// 使用した手札を削除するので、新しい手札カレントを設定する処理
	// 見えている手札UIカード上の右側(時計回りの次)を手札とする
	// ただし、見えている手札UIカードが最後尾の場合は左側(反時計回りの次)を手札とする
	if (std::next(visibleCurrentIt) == visibleCards_.end())
	{
		visibleCurrentIt--;
	}
	else
	{
		visibleCurrentIt++;
	}

	// UIカードを元に、手札からイテレータを見つけ出す
	handCurrent_ = GetSearchHandIt(*visibleCurrentIt);

	// 手札削除
	handCards_.erase(eraseHandIt);
	visibleCards_.erase(eraseVisibleIt);

}

void PlayerCardUI::DecideGoalAngle(void)
{
	//カードの範囲変数を更新する
	auto visibleIt = std::next(GetVisibleCurrentIt());
	for (; visibleIt != visibleCards_.end(); visibleIt++)
	{
		float currentAngle = (*visibleIt)->GetCurrentAngle();
		(*visibleIt)->SetStartAndGoalAngle(currentAngle - ARROUND_PER_RAD);
	}
}

void PlayerCardUI::ReloadAnimation(void)
{
	//リロード終了時は処理しない
	if (isReloadEnd_)
	{
		return;
	}
		
	//見せカードの0番目にリロードカードが来たら終了
	//endItをbeginの５個先(６枚目)に指定する
	//６枚目までイテレータを回す
	cardMoveCnt_ -= DELTA;
	//定期的に見せカード配列に格納する
	if (cardMoveCnt_ < 0.0f)
	{
		//サウンドを再生
		soundMng_.Play(ResourceManager::SRC::CARD_MOVE_SE, SoundManager::PLAYTYPE::BACK);

		//配列追加
		ReloadCardArray();

		//見せカードが7枚以上の時は終了
		if(visibleCards_.size()>VISIBLE_CARD_MAX&& reloadAnimCurr_==std::prev(initialCards_.end()))
		{
			isReloadEnd_ = true;
		}
		
		//リロード中カードを更新
		reloadAnimCurr_--;

		//先頭まで来たら最後尾に戻し、見せカードにリロードカードを追加
		if (reloadAnimCurr_ == initialCards_.begin())
		{
			(*reloadAnimCurr_)->SetCurrentAngle(static_cast<float>(-ARROUND_PER_RAD * PREV_CARD_COUNT));
			visibleCards_.emplace_front(*reloadAnimCurr_);

			auto insertIt = (*reloadAnimCurr_);
			insertIt->ResetCount();
			handCards_.emplace_front(insertIt);

			//リロードカードの対象を最後尾に戻す
			reloadAnimCurr_ = std::prev(initialCards_.end());
		}
		cardMoveCnt_ = CardUIController::RELOAD_MOVE_CARD_TIME_PER;

	}

	//見せカードの移動
	//座標を指定する時に使用する
	int i = 0;
	for (auto& card : visibleCards_)
	{
		card->SetStartAndGoalAngle(ARROUND_PER_RAD * (i - CARDS_BEFORE_CURRENT));
		card->MoveOnRevolver(cardMoveCnt_, CardUIController::RELOAD_MOVE_CARD_TIME_PER);
		i++;
	}

	//見せカードが7枚以上お時はポップ
	if(visibleCards_.size()>VISIBLE_CARD_MAX)
	{
		visibleCards_.pop_back();
	}
}

void PlayerCardUI::ReloadCardArray(void)
{
	//リロードカードの現在位置にセット
	(*reloadAnimCurr_)->SetCurrentAngle(static_cast<float>(-ARROUND_PER_RAD * PREV_CARD_COUNT));

	//見せるカード配列に追加
	visibleCards_.emplace_front(*reloadAnimCurr_);

	//手札配列に追加
	if (std::find(handCards_.begin(), handCards_.end(), *reloadAnimCurr_) == handCards_.end())
	{
		auto insertIt = (*reloadAnimCurr_);
		insertIt->ResetCount();
		handCards_.emplace_front(insertIt);
	}
}

void PlayerCardUI::MoveUpDownVisibleCards(void)
{
	//カードが動いていないときに上下に動かす
	if (selectState_ == CARD_SELECT::NONE || selectState_ == CARD_SELECT::RELOAD_WAIT)
	{
		for (auto& visible : visibleCards_)
		{
			visible->MoveUpDown();
		}
	}
}

void PlayerCardUI::DrawArrowAndBotton(void)
{

	//リボルバー回転方向の左方向矢印の描画
	DrawRotaGraphF(REVOLVER_ARROW_L_POS.x, REVOLVER_ARROW_L_POS.y
		, REVOLVER_ARROW_SCL, UtilityCommon::Deg2RadF(REVOLVER_ARROW_L_ANGLE), imgRevolverArrow_, true);

	//リボルバー回転方向の右方向矢印の描画
	DrawRotaGraphF(REVOLVER_ARROW_R_POS.x, REVOLVER_ARROW_R_POS.y
		, REVOLVER_ARROW_SCL, UtilityCommon::Deg2RadF(-REVOLVER_ARROW_L_ANGLE), imgRevolverArrow_, true,true,false);

	Vector2F btnPos = REVOLVER_ARROW_L_POS;
	btnPos.y -= REVOLVER_ARROW_SCL_SIZE.y / 2 + REVOLVER_BTN_ARROW_OFFSET;

	//移動矢印の描画
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::LBUTTON_NOPUSH, btnPos, REVOLVER_BTN_SIZE);
	btnPos = REVOLVER_ARROW_R_POS;
	btnPos.y -= REVOLVER_ARROW_SCL_SIZE.y / 2 + REVOLVER_BTN_ARROW_OFFSET;
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::RBUTTON_NOPUSH, btnPos, REVOLVER_BTN_SIZE);

}

std::list<std::shared_ptr<CardUIController>>::iterator PlayerCardUI::GetVisibleCurrentIt(void)
{
	auto it = std::find(visibleCards_.begin(), visibleCards_.end(), *handCurrent_);
	return it;
}

std::list<std::shared_ptr<CardUIController>>::iterator PlayerCardUI::GetSearchHandIt(
	std::shared_ptr<CardUIController> target)
{
	auto it = std::find(handCards_.begin(), handCards_.end(), target);
	return it;
}

