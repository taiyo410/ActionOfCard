#include "../pch.h"
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Common/Easing.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Card/CardUIDraw.h"
#include "CardUIController.h"

CardUIController::CardUIController(int& _cardNumImgs) :
	typeImg_(UtilityCommon::INITIAL_HANDLE),
	cardNoImg_(_cardNumImgs),
	status_(),
	upDownMoveAngle_(0.0f),
	disitionCnt_(0.0f),
	reactCnt_(0.0f),
	state_(CARD_STATE::DRAW_PILE),
	cardPos_({0.0f,0.0f}),
	currentAngle_(0.0f),
	goalAngle_(currentAngle_),
	cardScl_(1.0),
	sclCnt_(SCL_LERP_TIME),
	upDownMovePow_(),
	baseCardPos_(),
	numPos_(),
	startAngle_(),
	centerPos_(),
	halfSize_(),
	cardImg_()
{
}

CardUIController::~CardUIController(void)
{
}

void CardUIController::Load(void)
{
	cardDraw_ = std::make_unique<CardUIDraw>(cardImg_, cardPos_, cardScl_);
	cardDraw_->Load();
}

void CardUIController::Init(void)
{
	easing_ = std::make_unique<Easing>();
	cardDraw_->Init();
}
void CardUIController::Update(void)
{
	cardDraw_->Update();
}
void CardUIController::Draw(void)
{
	//カードの描画
	cardDraw_->Draw();
}

void CardUIController::DrawReloadGauge(const float& _reloadPer)
{
	cardDraw_->DrawReloadGauge(_reloadPer);
}

void CardUIController::SelectCardDrawFrame(void)
{
	cardDraw_->DrawSelectedFrame();
}

void CardUIController::DrawSelectCard(void)
{
	cardDraw_->DrawSelectCard();
}

void CardUIController::DecisionMove(void)
{
	//カード座標のイージング
	cardPos_ = easing_->EaseFunc(baseCardPos_, DISITON_CARD_POS,
		(DISITION_MOVE_CARD_TIME - disitionCnt_) / DISITION_MOVE_CARD_TIME, Easing::EASING_TYPE::CUBIC_OUT);

	//カウントのセット
	disitionCnt_ -= UtilityCommon::FIXED_DELTA_TIME;
}

void CardUIController::ReactMove(const Vector2F& _goalPos)
{
	//カード座標のイージング
	cardPos_ = easing_->EaseFunc(baseCardPos_, _goalPos,
		(REACT_MOVE_CARD_TIME - reactCnt_) / REACT_MOVE_CARD_TIME,Easing::EASING_TYPE::LERP);

	//カウントセット
	reactCnt_ -= UtilityCommon::FIXED_DELTA_TIME;
}

void CardUIController::ReactUpdate(const Vector2F& _goalPos)
{
	//すでに弾き状態だったら飛ばす
	if (state_ != CARD_STATE::REACT)return;

	//まだ決定移動中ならそちらを優先
	if (disitionCnt_ > 0.0f)
	{
		DecisionMove();
		return;
	}

	//弾かれ移動
	ReactMove(_goalPos);

	//弾き状態にする
	if (reactCnt_ <= 0.0f)
	{
		state_ = CARD_STATE::USED;
		sclCnt_ = 0.0f;
	}
}

void CardUIController::MoveOnRevolver(const float& _cnt,const float& moveTimeMax)
{
	float time = (moveTimeMax - _cnt) / moveTimeMax;
	float startRad = startAngle_;
	float goalRad = goalAngle_;

	//角度をイージングで補完
	currentAngle_ = easing_->EaseFuncRad(startRad
		, goalRad, time,Easing::EASING_TYPE::LERP);

	//カード座標を角度によって回す
	cardPos_.x = CENTER_X + std::sin(currentAngle_) * RADIUS_X;
	cardPos_.y = CENTER_Y - std::cos(currentAngle_) * RADIUS_Y;
}

void CardUIController::MoveUpDown(void)
{
	upDownMoveAngle_ += UP_DOWN_MOVE_SPEED;

	//Y座標をサイン波で動かす
	cardPos_.y = baseCardPos_.y + sin(upDownMoveAngle_) * UP_DOWN_MOVE_RADIUS;
}

void CardUIController::ChangeDicisionEnemyCardMove(void)
{
	cardPos_ = ENEMY_SELECT_CARD_START_POS;
	baseCardPos_ = ENEMY_SELECT_CARD_START_POS;
	numPos_ = cardPos_ + (NUM_LOCAL_POS * cardScl_);
}

void CardUIController::SetStartAndGoalAngle(const float& _goalrad)
{
	goalAngle_ = _goalrad; 
	startAngle_ = currentAngle_;
}

void CardUIController::SetStartAngle(void)
{
	startAngle_ = currentAngle_;
}

void CardUIController::ResetCount(void)
{
	sclCnt_ = SCL_LERP_TIME;
	disitionCnt_ = DISITION_MOVE_CARD_TIME;
	cardScl_ = 1.0f;
	reactCnt_ = REACT_MOVE_CARD_TIME;
	state_ = CARD_STATE::DRAW_PILE;
}

void CardUIController::EraseUsedCard(void)
{
	if (state_ != CARD_STATE::USED)return;

	//カードのスケールのイージング
	cardScl_ = easing_->EaseFunc(cardScl_, 0.0f, (SCL_LERP_TIME - sclCnt_) / SCL_LERP_TIME,Easing::EASING_TYPE::LERP);

	//カウント
	sclCnt_ -= static_cast<double>(UtilityCommon::FIXED_DELTA_TIME);
}

void CardUIController::InitCard(const int& _num)
{
	currentAngle_ = static_cast<float>(ARROUND_PER_RAD * _num - ARROUND_PER_RAD);
	startAngle_ = currentAngle_;
	cardPos_.x = CENTER_X + std::sin(currentAngle_) * RADIUS_X;
	cardPos_.y = CENTER_Y - std::cos(currentAngle_) * RADIUS_Y;
}

void CardUIController::ChangeUsedCard(void)
{
	if (state_ == CARD_STATE::REACT || state_ == CARD_STATE::USED||disitionCnt_ > 0.0f)return;
	state_ = CARD_STATE::USED;
}

void CardUIController::ChangeReactCard(void)
{
	if (state_ == CARD_STATE::USED || state_ == CARD_STATE::REACT)return;
	state_ = CARD_STATE::REACT;
	reactCnt_ = REACT_MOVE_CARD_TIME;
}

void CardUIController::ChangeUsing(void)
{
	if (state_ == CARD_STATE::USED || state_ == CARD_STATE::REACT)return;
	state_ = CARD_STATE::USING;
}