#include "pch.h"
#include "Utility/UtilityCommon.h"
#include "Utility/Utility2D.h"
#include "Utility/UtilityJson.h"
#include "Common/Easing.h"
#include "Manager/Resource/FontManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/ButtonUIManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Object/Card/CardSystem.h"
#include "Object/Character/UI/HpUI.h"
#include "Object/Card/PlayerCardUI.h"
#include "Object/Card/EnemyCardUI.h"
#include "Object/DirectionUI.h"
#include "UIManager.h"

UIManager::UIManager(void):
	resMng_(ResourceManager::GetInstance())
{
	CreateHpUI();
	CreateCardUI();

	directionUI_ = std::make_unique<DirectionUI>();
	easing_ = std::make_unique<Easing>();
}

void UIManager::CreateHpUI(void)
{
	std::unique_ptr<HpUI>hpUi;
	hpUi = std::make_unique<HpUI>(CHARACTER_TYPE::PLAYER);
	characterHpUI_[CHARACTER_TYPE::PLAYER] = std::move(hpUi);

	hpUi = std::make_unique<HpUI>(CHARACTER_TYPE::ENEMY);
	characterHpUI_[CHARACTER_TYPE::ENEMY] = std::move(hpUi);
}

void UIManager::CreateCardUI(void)
{
	std::unique_ptr<CardUIBase>cardUI;

	cardUI = std::make_unique<PlayerCardUI>();
	cardUI_[CHARACTER_TYPE::PLAYER] = std::move(cardUI);

	cardUI = std::make_unique<EnemyCardUI>();
	cardUI_[CHARACTER_TYPE::ENEMY] = std::move(cardUI);
}

void UIManager::Load(void)
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Load();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Load();
	}

	LoadJsonParameter();
	directionUI_->Load();

	fontHandle_= CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	higherImg_ = resMng_.Load(ResourceManager::SRC::HIGHER_IMG).handleId_;
	lowerImg_ = resMng_.Load(ResourceManager::SRC::LOWER_IMG).handleId_;
	upArrowImg_ = resMng_.Load(ResourceManager::SRC::WIN_ARROW_IMG).handleId_;
	downArrowImg_ = resMng_.Load(ResourceManager::SRC::LOSE_ARROW_IMG).handleId_;
}

void UIManager::Init(void)
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Init();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Init();
	}

	directionUI_->Init();
}

void UIManager::Update(void)
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Update();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Update();
	}

	EasingWinnerUISize();
}

void UIManager::DirectionUpdate(void)
{
	directionUI_->Update();
}

void UIManager::Draw(void)
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Draw();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Draw();
	}

	DrawAttackButtonAndDodgeButton();

	DrawHigherAndLower();
}

void UIManager::DirectionDraw(void)
{
	directionUI_->Draw();
}

void UIManager::RefreshHpUI(const CHARACTER_TYPE _charaType, const HP_DATA _hpData)
{
	characterHpUI_[_charaType]->RefreshHp(_hpData);
}

CardUIBase& UIManager::GetCardUI(const CHARACTER_TYPE _charaType)
{
	return *cardUI_[_charaType];
}

void UIManager::SetSkipPer(const float _skipPer)
{
	directionUI_->SetSkipGaugePer(_skipPer);
}

void UIManager::DrawAttackButtonAndDodgeButton(void)
{
	//UŒ‚‚Æ‰ñ”ðƒ{ƒ^ƒ“‚Ì•`‰æ
	Vector2F btnPos = topButtonPos_;
	ButtonUIManager::GetInstance().DrawFromLeftTop(ButtonUIManager::BTN_UI_TYPE::B_BUTTON_COL_PUSH, btnPos, buttonSize_);
	Vector2F strPos = { btnPos.x + buttonSize_ ,btnPos.y + FONT_SIZE / 2.0f };
	DrawStringFToHandle(strPos.x, strPos.y, attackStr_.c_str(), UtilityCommon::BLACK, fontHandle_);

	btnPos.y += buttonSize_ + buttonDistanceY_;
	strPos.y += buttonSize_ + buttonDistanceY_;
	DrawStringFToHandle(strPos.x, strPos.y, dodgeStr_.c_str(), UtilityCommon::BLACK, fontHandle_);
	ButtonUIManager::GetInstance().DrawFromLeftTop(ButtonUIManager::BTN_UI_TYPE::X_BUTTON_COL_PUSH, btnPos, buttonSize_);
}

void UIManager::DrawHigherAndLower(void)
{
	Utility2D::DrawGraphForCenter(higherImg_, higherImgPos_, higherImgScl_);
	Utility2D::DrawGraphForCenter(lowerImg_, lowerImgPos_, lowerImgScl_);

	Utility2D::DrawGraphForCenter(upArrowImg_, winnerArrowPos_, higherImgScl_);
	Utility2D::DrawGraphForCenter(downArrowImg_, loserArrowPos_, defaultHigherAndLowerScale_);
}

void UIManager::EasingWinnerUISize(void)
{
	CardSystem& cardSystem = CardSystem::GetInstance();
	if (cardSystem.GetJudgeRule() == CardSystem::ORDER_RULE::NORMAL)
	{
		UISizeEasing(higherImgScl_, winnerArrowPos_, higherImgPos_);
		loserArrowPos_ = lowerImgPos_ + arrowLocalPos_ * defaultHigherAndLowerScale_;
		lowerImgScl_ = defaultHigherAndLowerScale_;
	}
	else
	{
		UISizeEasing(lowerImgScl_, winnerArrowPos_, lowerImgPos_);
		loserArrowPos_ = higherImgPos_ - arrowLocalPos_ * defaultHigherAndLowerScale_;
		higherImgScl_ = defaultHigherAndLowerScale_;
	}
	scaleEaseCnt_ = scaleEaseCnt_ > easeTime_ ?
		0.0f : scaleEaseCnt_ + SceneManager::GetInstance().GetDeltaTime();
}

void UIManager::UISizeEasing(float& _scl, Vector2F& _arrowPos, const Vector2F& _winnerPos)
{
	_scl = easing_->EaseFunc(defaultHigherAndLowerScale_, easeGoalScl_, scaleEaseCnt_ / easeTime_, Easing::EASING_TYPE::QUAD_BACK);

	CardSystem& cardSystem = CardSystem::GetInstance();
	CardSystem::ORDER_RULE rule = cardSystem.GetJudgeRule();
	Vector2F arrowLocalPos = rule == CardSystem::ORDER_RULE::NORMAL ?
		arrowLocalPos_ * -1.0f : arrowLocalPos_;

	_arrowPos = _winnerPos + arrowLocalPos * _scl;

	scaleEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void UIManager::LoadJsonParameter(void)
{
	nlohmann::json jsonData = resMng_.Load(ResourceManager::SRC::UI_DATA).jsonData;

	for (const auto& [key, data] : jsonData.items())
	{
		if (key == "ActionButton")
		{
			topButtonPos_ = UtilityJson::GetLoadVector2F("topButtonPos", data);
			buttonDistanceY_ = data.value("distanceY", 0.0f);
			buttonSize_ = data.value("size", 0.0f);
			attackStr_ = UtilityCommon::GetWStringFromString(data.value("attackString", ""));
			dodgeStr_ = UtilityCommon::GetWStringFromString(data.value("dodgeString", ""));
		}
		else if (key == "HigherAndLower")
		{
			higherImgPos_ = UtilityJson::GetLoadVector2F("higherImagePos", data);
			lowerImgPos_ = UtilityJson::GetLoadVector2F("lowerImagePos", data);
			defaultHigherAndLowerScale_ = data.value("defaultScale", 0.0f);
			arrowLocalPos_ = UtilityJson::GetLoadVector2F("arrowPos", data);
			easeGoalScl_ = data.value("easingGoalScale", 0.0f);
			easeTime_ = data.value("easeTime", 0.0f);

		}
	}
}