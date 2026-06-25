#include "Utility/Utility3D.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Object/Common/AnimationController.h"
#include "Object/Card/CardSystem.h"
#include "Object/Card/CardDeck.h"
#include "Object/Card/CardBase.h"
#include "Object/Card/CardUIBase.h"
#include "Object/Character/Player/Player.h"
#include "Object/Character/Player/PlayerLogic.h"
#include "Object/Character/Base/ActionBase.h"
#include "./Idle.h"
#include "./Run.h"
#include "./React.h"
#include "./Dodge.h"
#include "./PlayerAction/PlayerCardAttackOneShort.h"
#include "./PlayerAction/PlayerCardAttackOneMiddle.h"
#include "./PlayerAction/PlayerCardAttackTwo.h"
#include "./PlayerAction/PlayerCardAttackThree.h"
#include "./PlayerAction/PlayerCardMagicFire.h"
#include "./PlayerAction/PlayerCardMagicThunder.h"
#include "./PlayerAction/PlayerCardReload.h"
#include "./EnemyAction/EnemyCardAttackJump.h"
#include "./EnemyAction/EnemyCardAttackStomp.h"
#include "./EnemyAction/EnemyCardReload.h"
#include "./EnemyAction/EnemyRevolutionAction.h"
#include "ActionController.h"

ActionController::ActionController(CharacterBase& _charaObj, LogicBase& _input, Transform& _trans, CardPresenter& _deck, AnimationController& _anim, InputManager::JOYPAD_NO _padNum) :
	character_(_charaObj),
	logic_(_input),
	trans_(_trans),
	cardPresent_(_deck),
	anim_(_anim),
	padNum_(_padNum),
	scnMng_(SceneManager::GetInstance()),
	act_(ACTION_TYPE::IDLE),
	isCardAct_(false),
	stepRotTime_(0.0f),
	speed_(0.0f),
	atkCombos_(),
	movePow_(Utility3D::VECTOR_ZERO),
	moveDir_(Utility3D::VECTOR_ZERO),
	dir_(Utility3D::VECTOR_ZERO),
	cardActionCnt_()
{
	actionTable_ = {
		{ACTION_TYPE::IDLE, [this]() {mainAction_.emplace(ACTION_TYPE::IDLE,std::make_unique<Idle>(*this,character_)); }},
		{ACTION_TYPE::RUN, [this]() {mainAction_.emplace(ACTION_TYPE::RUN,std::make_unique<Run>(*this,character_)); }},
		{ACTION_TYPE::DODGE,[this]() {mainAction_.emplace(ACTION_TYPE::DODGE,std::make_unique<Dodge>(*this,character_)); }},
		{ACTION_TYPE::REACT,[this]() {mainAction_.emplace(ACTION_TYPE::REACT,std::make_unique<React>(*this,character_)); }},
		{ACTION_TYPE::CARD_ATTACK_ONE_MIDDLE,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_ATTACK_ONE_MIDDLE,std::make_unique<PlayerCardAttackOneMiddle>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_ATTACK_ONE_SHORT,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_ATTACK_ONE_SHORT,std::make_unique<PlayerCardAttackOneShort>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_ATTACK_TWO,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_ATTACK_TWO,std::make_unique<PlayerCardAttackTwo>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_ATTACK_THREE,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_ATTACK_THREE,std::make_unique<PlayerCardAttackThree>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_MAGIC_FIRE,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_MAGIC_FIRE,std::make_unique<PlayerCardMagicFire>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_MAGIC_THUNDER,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_MAGIC_THUNDER,std::make_unique<PlayerCardMagicThunder>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_RELOAD,[this]() {
			if (character_.GetCharacterType() == CHARACTER_TYPE::PLAYER)
			{
				mainAction_.emplace(ACTION_TYPE::CARD_RELOAD, std::make_unique<PlayerCardReload>(*this, character_, cardPresent_));
			}
			else
			{
				mainAction_.emplace(ACTION_TYPE::CARD_RELOAD, std::make_unique<EnemyCardReload>(*this, character_, cardPresent_));
			}
		}},
		{ACTION_TYPE::CARD_ATTACK_ENEMY_JUMP,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_ATTACK_ENEMY_JUMP,std::make_unique<EnemyCardAttackJump>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::CARD_ATTACK_ENEMY_STOMP,[this]() {
				mainAction_.emplace(ACTION_TYPE::CARD_ATTACK_ENEMY_STOMP,std::make_unique<EnemyCardAttackStomp>(*this,character_,cardPresent_));
		}},
		{ACTION_TYPE::REVOLUTION,[this]() {
				mainAction_.emplace(ACTION_TYPE::REVOLUTION,std::make_unique<EnemyRevolutionAction>(*this,character_));
		}}
	};

	actionStrTable_= {
		{"idle", ACTION_TYPE::IDLE},
		{"run", ACTION_TYPE::RUN},
		{"react", ACTION_TYPE::REACT},
		{"dodge", ACTION_TYPE::DODGE},
		{"react", ACTION_TYPE::REACT},
		{"attack_1_Middle", ACTION_TYPE::CARD_ATTACK_ONE_MIDDLE},
		{"attack_1_Short", ACTION_TYPE::CARD_ATTACK_ONE_SHORT},
		{"attack_2", ACTION_TYPE::CARD_ATTACK_TWO},
		{"attack_3", ACTION_TYPE::CARD_ATTACK_THREE},
		{"fireMagic", ACTION_TYPE::CARD_MAGIC_FIRE},
		{"thunderMagic", ACTION_TYPE::CARD_MAGIC_THUNDER},
		{"cardReload", ACTION_TYPE::CARD_RELOAD},
		{"stompAttack", ACTION_TYPE::CARD_ATTACK_ENEMY_STOMP},
		{"jumpAttack", ACTION_TYPE::CARD_ATTACK_ENEMY_JUMP },
		{"revolution", ACTION_TYPE::REVOLUTION },
	};
};

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

	//革命までのアクション回数をランダム指定
	cardActionMaxCnt_ = UtilityCommon::GetMersenneRandomNumber(REVOLUTION_ACT_COUNT_MIN, REVOLUTION_ACT_COUNT_MAX);

	isCardAct_ = false;
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
	character_.Rotate();

	//移動量の更新
	DirAndMovePowUpdate();

	//カードの移動
	CardMove();
}


void ActionController::AddAction(void)
{
	const auto datas = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARA_DATA).jsonData;

	//読み込むキャラクター種類
	std::string charaStr = character_.GetObjectName();

	//UseActionを読み込んで追加する
	if (datas[charaStr].contains("UseAction"))
	{
		for (const auto& actData : datas[charaStr]["UseAction"])
		{
			std::string actionName = actData.get<std::string>();
			auto it = actionStrTable_.find(actionName);
			if (it != actionStrTable_.end())
			{
				actionTable_[it->second]();
			}
		}
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

void ActionController::AnimLoadNotify(const ACTION_LOAD_DATA& animVar)
{
	//全行動クラス一斉に通知する
	for(auto& [type,action]:mainAction_)
	{
		action->LoadAnimVar(animVar);
	}
}

void ActionController::DecideCardAction(void)
{
	//リロードカードであればリロードへ移行
	if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::RELOAD)
	{
		ChangeAction(ACTION_TYPE::CARD_RELOAD);
		return;
	}
	//敵はカード攻撃処理へ
	if (character_.GetCharaType() == CHARACTER_TYPE::ENEMY)
	{
		DecideEnemyCardAction();
		return;
	}

	//プレイヤーはカードによって攻撃を遷移する
	if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		//手札に移動
		cardPresent_.PutCard();

		//攻撃アクションの遷移
		DecideAttackOne();
	}
	else if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::FIRE)
	{
		//手札に移動
		cardPresent_.PutCard();
		ChangeAction(ACTION_TYPE::CARD_MAGIC_FIRE);
	}
	else if (cardPresent_.GetCardType() == CardBase::CARD_TYPE::THUNDER)
	{
		//手札に移動
		cardPresent_.PutCard();
		ChangeAction(ACTION_TYPE::CARD_MAGIC_THUNDER);
	}

}
void ActionController::ChangeComboCardAttack(void)
{
	//空の場合、攻撃不可能な場合はアイドル状態へ
	if (atkCombos_.empty()||!IsAttackable()||cardPresent_.GetCardType() == CardBase::CARD_TYPE::RELOAD)
	{
		//コンボ情報を空にする
		if(!atkCombos_.empty()) atkCombos_.pop();
		//コンボ情報がなければアイドル状態へ
		ChangeAction(ACTION_TYPE::IDLE);
		return;
	}

	//次の攻撃アクションに遷移
	const ACTION_TYPE& atkType = atkCombos_.front();
	ChangeAction(atkType);

	//攻撃に使用したカードを手札に戻す
	cardPresent_.PutCard();

	//コンボ情報をポップ
	atkCombos_.pop();
}

void ActionController::ComboInput(void)
{
	//現在のカードの種類を取得
	const CardBase::CARD_TYPE type = cardPresent_.GetCardType();

	//入力されていない、または、コンボ配列に何か挿入されている時は処理を飛ばす
	if (!logic_.GetIsAct().isCardUse||!atkCombos_.empty()||type==CardBase::CARD_TYPE::RELOAD)return;

	//次の段階の攻撃を入れる
	if(type==CardBase::CARD_TYPE::ATTACK)
	{
		if(act_==ACTION_TYPE::CARD_ATTACK_ONE_MIDDLE||act_==ACTION_TYPE::CARD_ATTACK_ONE_SHORT)
		{
			atkCombos_.push(ACTION_TYPE::CARD_ATTACK_TWO);
		}
		else if(act_ == ACTION_TYPE::CARD_ATTACK_TWO)
		{
			atkCombos_.push(ACTION_TYPE::CARD_ATTACK_THREE);
		}
	}
	else if(type==CardBase::CARD_TYPE::FIRE)
	{
		atkCombos_.push(ACTION_TYPE::CARD_ATTACK_THREE);
	}
	else
	{
		return;
	}
}

void ActionController::CancelCardActionByDodge(void)
{
	//回避でアクションを中断
	if (logic_.GetIsAct().isDodge
		&& cardPresent_.GetCardUIState() != CardUIBase::CARD_SELECT::DISITION)
	{
		ChangeAction(ActionController::ACTION_TYPE::DODGE);
	}
}

void ActionController::CardMove(void)
{
	CardUIBase::CARD_SELECT uiState = cardPresent_.GetCardUIState();
	if (uiState != CardUIBase::CARD_SELECT::NONE)return;
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
	//補完角度の設定(入力角度まで方向転換する)
	character_.SetGoalRotate();
}

void ActionController::DirAndMovePowUpdate(void)
{
	//方向の更新
	moveDir_ = character_.GetRotation().dir_;
	float speed = mainAction_[act_]->GetSpeed();
	//移動量の更新
	movePow_ = VScale(moveDir_, speed);
}
const bool ActionController::IsCardLeftMoveable(void)
{
	//カードUIの選択状態
	const CardUIBase::CARD_SELECT selectState = cardPresent_.GetCardUIState();

	return logic_.GetIsAct().isCardMoveLeft 
		&& selectState != CardUIBase::CARD_SELECT::RELOAD;
}
const bool ActionController::IsCardRightMoveable(void)
{
	//カードUIの選択状態
	CardUIBase::CARD_SELECT selectState = cardPresent_.GetCardUIState();

	return logic_.GetIsAct().isCardMoveRight 
		&& selectState != CardUIBase::CARD_SELECT::RELOAD;
}

void ActionController::DecideAttackOne(void)
{
	//相手との距離を取得
	const float dis = logic_.GetTargetDis();

	//相手との距離が一定以下なら攻撃する
	const float MIDDLE_DIS = 300.0f;

	//距離によって攻撃アクション遷移
	if (dis >= MIDDLE_DIS)
	{
		ChangeAction(ACTION_TYPE::CARD_ATTACK_ONE_MIDDLE);
	}
	else
	{
		ChangeAction(ACTION_TYPE::CARD_ATTACK_ONE_SHORT);
	}
}

void ActionController::DecideEnemyCardAction(void)
{
	if (cardActionCnt_ >= cardActionMaxCnt_)
	{
		//革命へ
		cardActionCnt_ = 0;
		//革命までのアクション回数をランダム指定
		cardActionMaxCnt_ = UtilityCommon::GetMersenneRandomNumber(REVOLUTION_ACT_COUNT_MIN, REVOLUTION_ACT_COUNT_MAX);
		ChangeAction(ACTION_TYPE::REVOLUTION);
		return;
	}

	const float distance = logic_.GetTargetDis();

	//ランダムの数値取得
	int rand = GetRand(UtilityCommon::PERCENT_MAX);
	if (distance > ATK_DISTANCE)
	{
		//遠距離時
		if (rand > STOMP_WEIGHT)
		{
			//手札に移動
			cardPresent_.PutCard();

			//敵の岩攻撃
			ChangeAction(ACTION_TYPE::CARD_ATTACK_ENEMY_STOMP);

			//カードアクション回数のカウント
			cardActionCnt_++;
		}
		else if (rand <= JUMP_WEIGHT)
		{
			//手札に移動
			cardPresent_.PutCard();

			//ジャンプ
			ChangeAction(ACTION_TYPE::CARD_ATTACK_ENEMY_JUMP);

			//カードアクション回数のカウント
			cardActionCnt_++;
		}
	}
	else
	{
		//近距離時
		if (rand >= STOMP_WEIGHT)
		{
			//手札に移動
			cardPresent_.PutCard();

			//敵の岩攻撃
			ChangeAction(ACTION_TYPE::CARD_ATTACK_ENEMY_STOMP);

			//カードアクション回数のカウント
			cardActionCnt_++;
		}
		else if (rand < JUMP_WEIGHT)
		{
			//手札に移動
			cardPresent_.PutCard();

			//ジャンプ
			ChangeAction(ACTION_TYPE::CARD_ATTACK_ENEMY_JUMP);

			//カードアクション回数のカウント
			cardActionCnt_++;
		}	
	}
	logic_.SetIsActioning(true);
}

const bool ActionController::IsAttackable(void)
{
	std::vector<CardBase::CARD_TYPE>cardTypes = cardPresent_.GetHandCardType();
	int handCardTypeSize = static_cast<int>(cardPresent_.GetHandCardType().size());
	return handCardTypeSize == 1 && cardTypes[0] == CardBase::CARD_TYPE::ATTACK
		&&cardPresent_.GetCardUIState() != CardUIBase::CARD_SELECT::DISITION
		&& IsCardDecisionControl();
}