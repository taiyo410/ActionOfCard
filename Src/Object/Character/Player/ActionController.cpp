#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"./PlayerLogic.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include"../Object/Card/CardSystem.h"
#include "../../../Object/Common/AnimationController.h"
#include"../../Card/CardDeck.h"
#include"../../Card/CardBase.h"
#include"../../Card/CardUIBase.h"
#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/React.h"
#include"../Action/Dodge.h"
#include"../Action/PlayerCardAction.h"
#include"../Action/EnemyCardAction.h"

#include "ActionController.h"

ActionController::ActionController(CharacterBase& _charaObj, LogicBase& _input, Transform& _trans, CardPresenter& _deck, AnimationController& _anim, InputManager::JOYPAD_NO _padNum) :
	charaObj_(_charaObj)
	, logic_(_input)
	, trans_(_trans)
	, cardPresent_(_deck)
	, anim_(_anim)
	, padNum_(_padNum)
	, scnMng_(SceneManager::GetInstance())
	, act_(ACTION_TYPE::IDLE)
	, cardActTime_(0.0f)
	, isCardAct_(false)
	, stepRotTime_(0.0f)
	, speed_(0.0f)
	, movePow_(Utility3D::VECTOR_ZERO)
	, moveDir_(Utility3D::VECTOR_ZERO)
	,dir_(Utility3D::VECTOR_ZERO)
{
	//エフェクト
	ResourceManager::SRC footSE = charaObj_.GetFootSE();
	const float& footSEDisCount = charaObj_.GetFootSEDisCount();
	const float& speed = charaObj_.GetStatus().speed;
	VECTOR dir = trans_.GetForward();
	actionTable_ = {
		{ACTION_TYPE::IDLE, [this]() {mainAction_.emplace(ACTION_TYPE::IDLE,std::make_unique<Idle>(*this)); }},
		{ACTION_TYPE::MOVE, [this,speed,footSE,footSEDisCount]() {mainAction_.emplace(ACTION_TYPE::MOVE,std::make_unique<Run>(*this,speed,footSE,footSEDisCount)); }},
		{ACTION_TYPE::DODGE,[this]() {mainAction_.emplace(ACTION_TYPE::DODGE,std::make_unique<Dodge>(*this,trans_,charaObj_.GetStatus().speed)); }},
		{ACTION_TYPE::REACT,[this]() {mainAction_.emplace(ACTION_TYPE::REACT,std::make_unique<React>(*this)); }},
		{ACTION_TYPE::CARD_ACTION,[this]() {
			if (charaObj_.GetCharaType() == CHARACTER_TYPE::PLAYER)
			{
				mainAction_.emplace(ACTION_TYPE::CARD_ACTION,std::make_unique<PlayerCardAction>(*this,charaObj_,cardPresent_));
			}
			else
			{
				mainAction_.emplace(ACTION_TYPE::CARD_ACTION,std::make_unique<EnemyCardAction>(*this,charaObj_,cardPresent_));
			}
		}},
	};
}

ActionController::~ActionController(void)
{
	StopResource();
	mainAction_.clear();
	subAction_.clear();
	changeAction_.clear();
}

void ActionController::Init(void)
{

	auto cntl = InputManager::CONTROLL_TYPE::ALL;

	//初期化の前に追加したアクションの初期化
	mainAction_[act_]->Init();

	isCardAct_ = false;
	cardActTime_ = 0.0f;
}

void ActionController::Load(void)
{
	for (auto& action : mainAction_)
	{
		action.second->Load();
	}
}

void ActionController::Update(void)
{
	mainAction_[act_]->Update();

	//入力方向に応じた方向転換
	MoveDirFromInput();

	//キャラクターの回転
	charaObj_.Rotate();

	//移動量の更新
	DirAndMovePowUpdate();

	//カードの移動
	CardMove();
}


void ActionController::AddAction(std::vector<ACTION_TYPE> _types)
{
	for (auto& type : _types)
	{
		actionTable_[type]();
	}
}

ActionBase& ActionController::GetMainAction(void)
{
	return *mainAction_.at(act_);
}

#ifdef _DEBUG
void ActionController::DrawDebug(void)
{
}
#endif // _DEBUG

const float& ActionController::GetSpd(void) const
{
	return mainAction_.at(act_)->GetSpeed();
}

void ActionController::ChangeAction(const ACTION_TYPE _act)
{
	if (act_ == _act)return;
	ACTION_TYPE prevType = act_;
	mainAction_[prevType]->Release();
	act_ = _act;
	mainAction_[act_]->Init();
}

void ActionController::CardMove(void)
{
	//CardUIBase& cardUI = charaObj_.GetCardUI();
	CardUIBase::CARD_SELECT uiState = cardPresent_.GetCardUIState();
	if (uiState == CardUIBase::CARD_SELECT::DISITION
		|| uiState == CardUIBase::CARD_SELECT::LEFT
		|| uiState == CardUIBase::CARD_SELECT::RIGHT)return;
	if (IsCardLeftMoveable())
	{
		cardPresent_.RoleRevolver(CardUIBase::CARD_SELECT::LEFT);
	}
	else if (IsCardRightMoveable())
	{
		cardPresent_.RoleRevolver(CardUIBase::CARD_SELECT::RIGHT);
	}
}

const Quaternion ActionController::GetPlayerRotY(void)
{
	return playerRotY_;
}

void ActionController::StopResource(void)
{
}

const bool ActionController::IsCardDecisionControl(void)
{
	const CardUIBase::CARD_SELECT selectState = cardPresent_.GetCardUIState();
	return selectState == CardUIBase::CARD_SELECT::NONE;
}

void ActionController::SetFlinchCnt(const float _flinchTime)
{
	mainAction_[ACTION_TYPE::REACT]->SetFlinchCnt(_flinchTime);
}

void ActionController::MoveDirFromInput(void)
{
	charaObj_.MoveDirFromInput();

	if (mainAction_[act_]->GetIsTurnable())
	{
		//補完角度の設定(入力角度まで方向転換する)
		charaObj_.SetGoalRotate();
	}
}

void ActionController::DirAndMovePowUpdate(void)
{
	//方向の更新
	moveDir_ = charaObj_.GetRotation().dir_;
	float speed = mainAction_[act_]->GetSpeed();
	//移動量の更新
	movePow_ = VScale(moveDir_, speed);
}
const bool ActionController::IsCardLeftMoveable(void)
{
	//カードアクション種類
	const ActionBase::CARD_ACT_TYPE cardAct = mainAction_.at(act_)->GetCardAction();

	//カードUIの選択状態
	const CardUIBase::CARD_SELECT selectState = cardPresent_.GetCardUIState();

	return logic_.GetIsAct().isCardMoveLeft 
		&& cardAct != ActionBase::CARD_ACT_TYPE::RELOAD
		&& selectState !=CardUIBase::CARD_SELECT::RELOAD;
}
const bool ActionController::IsCardRightMoveable(void)
{
	//カードアクション種類
	const ActionBase::CARD_ACT_TYPE cardAct = mainAction_.at(act_)->GetCardAction();

	//カードUIの選択状態
	CardUIBase::CARD_SELECT selectState = cardPresent_.GetCardUIState();

	return logic_.GetIsAct().isCardMoveRight 
		&& cardAct != ActionBase::CARD_ACT_TYPE::RELOAD
		&& selectState != CardUIBase::CARD_SELECT::RELOAD;
}
const ActionBase::CARD_ACT_TYPE& ActionController::GetCardAction(void)const
{
	return mainAction_.at(act_)->GetCardAction();
}

