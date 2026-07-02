#include "pch.h"
#include <nlohmann/json.hpp>
#include "Utility/UtilityCommon.h"
#include "Utility/Utility2D.h"
#include "Utility/UtilityJson.h"
#include "Common/Easing.h"
#include "Manager/Resource/FontManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/ButtonUIManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Renderer/PixelMaterial.h"
#include "Renderer/PixelRenderer.h"
#include "Object/Card/CardSystem.h"
#include "Object/Character/UI/HpUI.h"
#include "Object/Card/PlayerCardUI.h"
#include "Object/Card/EnemyCardUI.h"
#include "Object/DirectionUI.h"
#include "UIManager.h"

UIManager::UIManager(void) :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	isEndRevolution_(false),
	waitCnt_(WAIT_TIME),
	winUIImg_(UtilityCommon::INITIAL_HANDLE),
	isWinDirection_(false),
	winUIScl_(1.0)
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
	LoadRevolutionPostEffect();
	directionUI_->Load();

	fontHandle_= CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	higherImg_ = resMng_.Load(ResourceManager::SRC::HIGHER_IMG).handleId_;
	lowerImg_ = resMng_.Load(ResourceManager::SRC::LOWER_IMG).handleId_;
	winUIImg_ = resMng_.Load(ResourceManager::SRC::WIN_IMG).handleId_;
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

	//革命状態の初期化
	revolutionFadeFunc_ = [this]() {RevolutionInvertFadeNone(); }; 
	winUIDirectionFunc_ = [this]() { WINUIStompNone(); };
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

	UpdateRevolutionDirection();

	EasingWinnerUISize();

	UpdateWinUIDirection();
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

	DrawWinDirection();
}

void UIManager::DeleteRevolutionPostEffect(void)
{
	scnMng_.DeletePostEffect(invertRenderer_);
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

void UIManager::StartWINDirection(void)
{
	if (isWinDirection_)return;
	isWinDirection_ = true;
	winAlpha_ = 0;
	winEaseCnt_ = 0.0f;
	winUIWaitCnt_ = 0.0f;
	winUIDirectionFunc_ = [this]() {WINUIStompIn(); };
}

void UIManager::DrawAttackButtonAndDodgeButton(void)
{
	//攻撃と回避ボタンの描画
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
	CardSystem& cardSystem = CardSystem::GetInstance();
	CardSystem::ORDER_RULE rule = cardSystem.GetJudgeRule();
	int drawRule = rule == CardSystem::ORDER_RULE::NORMAL ? higherImg_ : lowerImg_;

	Utility2D::DrawGraphForCenter(drawRule, WIN_RULE_UI_POS, higherImgScl_);
}

void UIManager::DrawWinDirection(void)
{
	if (winAlpha_ == 0.0f)return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, winAlpha_);
	DrawRotaGraphF(winUIPos_.x, winUIPos_.y,winUIScl_, 0.0f, winUIImg_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, winAlpha_);
}

void UIManager::EasingWinnerUISize(void)
{
	UISizeEasing(higherImgScl_, higherImgPos_);
	CardSystem& cardSystem = CardSystem::GetInstance();
	scaleEaseCnt_ = scaleEaseCnt_ > easeTime_ ?
		0.0f : scaleEaseCnt_ + SceneManager::GetInstance().GetDeltaTime();
}

void UIManager::UISizeEasing(float& _scl,const Vector2F& _winnerPos)
{
	_scl = easing_->EaseFunc(defaultHigherAndLowerScale_, easeGoalScl_, scaleEaseCnt_ / easeTime_, Easing::EASING_TYPE::QUAD_BACK);

	scaleEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void UIManager::LoadRevolutionPostEffect(void)
{
	invertMaterial_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::REVOLUTION_POSTEFF_PS);

	//メインスクリーン
	const int mainScreen = scnMng_.GetMainScreen();
	invertMaterial_->AddTextureBuf(mainScreen);

	//マスク画像
	const int maskImg = resMng_.Load(ResourceManager::SRC::REVERSE_FADE_MASK).handleId_;
	invertMaterial_->AddTextureBuf(maskImg);
	invertMaterial_->AddConstBuf({ fadeCnt_ ,0.0f,0.0f,0.0f });

	invertRenderer_ = std::make_shared<PixelRenderer>(*invertMaterial_);
	invertRenderer_->MakeScreenVertex();
}

void UIManager::UpdateRevolutionDirection(void)
{
	//革命更新
	revolutionFadeFunc_();

	//シェーダー関連の更新
	const int mainScreen = scnMng_.GetMainScreen();
	invertMaterial_->SetTextureBuf(0, mainScreen);
	invertMaterial_->SetConstBuf(0, { fadeCnt_ / FADE_TIME ,0.0f,0.0f,0.0f });
}

void UIManager::UpdateWinUIDirection(void)
{
	if (!isWinDirection_)return;
	winUIDirectionFunc_();
}

void UIManager::RevolutionInvertFadeNone(void)
{
	//革命開始したらフェード開始
	if (isStartRevolution_)
	{
		scnMng_.SetPostEffect(invertRenderer_); 
		revolutionFadeFunc_ = [this]() {RevolutionInvertFadeIn(); };
	}
}

void UIManager::RevolutionInvertFadeIn(void)
{
	if (fadeCnt_ < FADE_TIME)
	{
		fadeCnt_ += scnMng_.GetDeltaTime();
		return;
	}

	//フェード終了後、一定時間待機
	if (waitCnt_ < 0.0f)
	{
		waitCnt_ = WAIT_TIME;
		revolutionFadeFunc_ = [this]() {RevolutionInvertFadeOut(); };
		//ルールを切り替える
		CardSystem::GetInstance().ChangeJudgeRule();
		return;
	}
	waitCnt_ -= scnMng_.GetDeltaTime();
}

void UIManager::RevolutionInvertFadeOut(void)
{
	if (fadeCnt_ > 0.0f)
	{
		fadeCnt_ -= scnMng_.GetDeltaTime();
	}
	else
	{
		scnMng_.DeletePostEffect(invertRenderer_);
		fadeCnt_ = 0.0f;
		isStartRevolution_ = false;
		isEndRevolution_ = true;
		revolutionFadeFunc_ = [this]() {RevolutionInvertFadeNone(); };
	}
}

void UIManager::WINUIStompNone(void)
{
}

void UIManager::WINUIStompIn(void)
{
	if (winUIWaitCnt_ > 0.0f)
	{
		winUIWaitCnt_ -= scnMng_.GetDeltaTime();
		if (winUIWaitCnt_ <= 0.0f)
		{
			winUIWaitCnt_ = 0.0f;
			winEaseCnt_ = 0.0f;
			winUIDirectionFunc_ = [this]() {WINUIStompOut(); };
		}
		return;
	}
	//スタンプイン演出
	StompInDirection(winEaseCnt_, winAlpha_, winUIScl_, true
		, winUIStartScl_, winUIEndScl_, winUIEaseTime_);

	if (winEaseCnt_ >= winUIEaseTime_)winUIWaitCnt_ = winUIWaitTime_;
}

void UIManager::WINUIStompOut(void)
{
	//スタンプアウト演出
	StompInDirection(winEaseCnt_, winAlpha_, winUIScl_, false
		, winUIStartScl_, winUIEndScl_, winUIEaseTime_);
	if (winEaseCnt_ >= winUIEaseTime_)
	{
		winEaseCnt_ = 0.0f;
		isWinDirection_ = false;
		winAlpha_ = 0;
		winUIDirectionFunc_ = [this]() {WINUIStompNone(); };
	}
}

void UIManager::StompInDirection(float& _easeCnt, int& _alpha,float& _scl, const bool _isStompIn, const float _startSize, const float _endSize, const float _time)
{
	// スタンプ演出のイージング計算
	if (_easeCnt <= _time)
	{
		_easeCnt += SceneManager::GetInstance().GetDeltaTime();
		if (_easeCnt > _time) _easeCnt = _time;

		float t = _easeCnt / _time;

		// サイズの補間(スタンプインなら大→小、スタンプアウトなら小→大)
		_scl = _isStompIn ? easing_->EaseFunc(_startSize, _endSize, t, Easing::EASING_TYPE::QUAD_IN) :
			easing_->EaseFunc(_endSize, _startSize, t, Easing::EASING_TYPE::QUAD_IN);

		// アルファ値の補間
		_alpha = _isStompIn ? easing_->EaseFunc(UtilityCommon::ALPHA_MIN, UtilityCommon::ALPHA_MAX, t, Easing::EASING_TYPE::LERP) :
			easing_->EaseFunc(UtilityCommon::ALPHA_MAX, UtilityCommon::ALPHA_MIN, t, Easing::EASING_TYPE::LERP);

		//カウントの減算
		_easeCnt += scnMng_.GetDeltaTime();
	}
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
			std::string str = data.value("attackString", "");
			str = UtilityCommon::ConvertUtf8ToSJIS(str);
			attackStr_ = UtilityCommon::GetWStringFromString(str);
			str = data.value("dodgeString", "");
			str = UtilityCommon::ConvertUtf8ToSJIS(str);
			dodgeStr_ = UtilityCommon::GetWStringFromString(str);
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
		else if (key == "CardWinUI")
		{
			winUIEaseTime_ = data.value("winUIEaseTime", 0.0f);
			winUIStartScl_ = data.value("winUIStartScale", 0.0f);
			winUIEndScl_ = data.value("winUIEndScale", 0.0f);
			winUIWaitTime_ = data.value("winUIWaitTime", 0.0f);
			winUIPos_ = UtilityJson::GetLoadVector2F("winUIPos", data);
		}
	}
}