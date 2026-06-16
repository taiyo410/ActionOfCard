#include "pch.h"
#include "Utility/UtilityCommon.h"
#include "Utility/Utility2D.h"
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

void UIManager::DrawAttackButtonAndDodgeButton(void)
{
	//UŒ‚‚Æ‰ñ”ðƒ{ƒ^ƒ“‚Ì•`‰æ
	Vector2F btnPos = INIT_BOTTON_POS;
	ButtonUIManager::GetInstance().DrawFromLeftTop(ButtonUIManager::BTN_UI_TYPE::B_BUTTON_COL_PUSH, btnPos, BOTTON_SIZE);
	Vector2F strPos = { btnPos.x + BOTTON_SIZE ,btnPos.y + FONT_SIZE / 2.0f };
	DrawStringFToHandle(strPos.x , strPos.y, ATTACK_BTN_STR.c_str(), UtilityCommon::BLACK, fontHandle_);

	btnPos.y += BOTTON_SIZE + BOTTON_DISTANCE;
	strPos.y += BOTTON_SIZE + BOTTON_DISTANCE;
	DrawStringFToHandle(strPos.x, strPos.y, DODGE_BTN_STR.c_str(), UtilityCommon::BLACK, fontHandle_);
	ButtonUIManager::GetInstance().DrawFromLeftTop(ButtonUIManager::BTN_UI_TYPE::X_BUTTON_COL_PUSH, btnPos, BOTTON_SIZE);

	higherPos_ = { Application::SCREEN_HALF_X - 120.0f,80.0f };
	lowerPos_ = { Application::SCREEN_HALF_X + 120.0f,80.0f };

	constexpr float scl = 0.2f;
	CardSystem& cardSystem = CardSystem::GetInstance();
	if (cardSystem.GetJudgeRule() == CardSystem::ORDER_RULE::NORMAL)
	{
		higherImgScl_ = easing_->EaseFunc(scl, 0.24f, scaleEaseCnt_ / 0.5f, Easing::EASING_TYPE::QUAD_BACK);
		lowerImgScl_ = scl;
	}
	else
	{
		lowerImgScl_ = easing_->EaseFunc(scl, 0.24f, scaleEaseCnt_ / 0.5f, Easing::EASING_TYPE::QUAD_BACK);
		higherImgScl_ = scl;
	}
	scaleEaseCnt_ = scaleEaseCnt_ > 0.5f ? 0.0f : scaleEaseCnt_ + SceneManager::GetInstance().GetDeltaTime();

	Utility2D::DrawGraphForCenter(higherImg_, higherPos_, higherImgScl_);
	Utility2D::DrawGraphForCenter(lowerImg_, lowerPos_, lowerImgScl_);

	Vector2F arrowLocalPos = { 350,0 };
	arrowLocalPos *= higherImgScl_;
	Utility2D::DrawGraphForCenter(upArrowImg_, higherPos_- arrowLocalPos, higherImgScl_);
	Utility2D::DrawGraphForCenter(downArrowImg_, lowerPos_+ arrowLocalPos, scl);
}

void UIManager::DrawWinLoseUI(void)
{

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

	directionUI_->Load();

	fontHandle_= CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	higherImg_ = resMng_.Load(ResourceManager::SRC::HIGHER_IMG).handleId_;
	lowerImg_ = resMng_.Load(ResourceManager::SRC::LOWER_IMG).handleId_;
	upArrowImg_ = resMng_.Load(ResourceManager::SRC::WIN_ARROW_IMG).handleId_;
	downArrowImg_ = resMng_.Load(ResourceManager::SRC::LOSE_ARROW_IMG).handleId_;
}

void UIManager::Init()
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

void UIManager::Update()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Update();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Update();
	}
}

void UIManager::DirectionUpdate(void)
{
	directionUI_->Update();
}

void UIManager::Draw()
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