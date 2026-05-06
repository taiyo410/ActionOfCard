#include"../Object/Character/Player/Player.h"
#include"../Utility/Utility3D.h"
#include "../Object/Character/Base/CharacterBase.h"

#include "../../../Manager/Generic/Camera.h"
#include "PlayerLogic.h"

PlayerLogic::PlayerLogic(Transform& _myTrans,bool& _isCanMoveAble , InputManager::JOYPAD_NO _padNum, InputManager::CONTROLL_TYPE _cntl):
	LogicBase(_myTrans),
	isCanMoveable_(_isCanMoveAble),
	padNum_(_padNum),
	cntl_(_cntl),
	leftStickX_(-1),
	leftStickY_(-1),
	camera_(SceneManager::GetInstance().GetCamera()),
	input_(InputManager::GetInstance()),
	inputS_(InputManagerS::GetInstance())
{
	actCntl_ = ACT_CNTL::NONE;
	stickDeg_ = -1;

	inputUpdates_[InputManager::CONTROLL_TYPE::ALL] = [this]() {InputAll(); };
	inputUpdates_[InputManager::CONTROLL_TYPE::CONTROLLER] = [this]() {InputPad(); };
}

PlayerLogic::~PlayerLogic(void)
{
}

void PlayerLogic::Init(void)
{
	actCntl_ = ACT_CNTL::NONE;
	stickDeg_ = -1;
	moveDir_ = myTrans_.GetDir(Utility3D::DIR_F);
}

void PlayerLogic::Update(void)
{
	inputUpdates_[cntl_]();
}

void PlayerLogic::InputKeyBoard(void)
{
	
	//移動角度を決める
	if (input_.IsNew(MOVE_FRONT_KEY))
	{ 
		moveDir_ = Utility3D::DIR_F;

	}
	else if (input_.IsNew(MOVE_LEFT_KEY))
	{  
		moveDir_ = Utility3D::DIR_L;
	} 
	else if (input_.IsNew(MOVE_BACK_KEY))
	{ 
		moveDir_ = Utility3D::DIR_B;
	}
	else if (input_.IsNew(MOVE_RIGHT_KEY))
	{
		moveDir_ = Utility3D::DIR_R;
	}

	//カード使用
	if (input_.IsTrgDown(CARD_CHARGE_KEY)) { actCntl_ = ACT_CNTL::CARD_CHARGE; }

	//ジャンプキー
	if (input_.IsTrgDown(CARD_USE_KEY)) { actCntl_ = ACT_CNTL::CARD_USE; }
}

void PlayerLogic::InputAll(void)
{
	isAct_ = {};
	inputDir_ = {};

	//移動角度を決める
	if (input_.IsNew(MOVE_FRONT_KEY))
	{
		isAct_.isRun = true;
		inputDir_ = Utility3D::DIR_F;
	}
	else if (input_.IsNew(MOVE_LEFT_KEY))
	{
		isAct_.isRun = true;
		inputDir_ = Utility3D::DIR_L;
	}
	else if (input_.IsNew(MOVE_BACK_KEY))
	{
		isAct_.isRun = true;
		inputDir_ = Utility3D::DIR_B;
	}
	else if (input_.IsNew(MOVE_RIGHT_KEY))
	{
		isAct_.isRun = true;
		inputDir_ = Utility3D::DIR_R;
	}

	//スティックが傾いたら
	if (inputS_.IsPressed(INPUT_EVENT::UP) || inputS_.IsPressed(INPUT_EVENT::DOWN)
		|| inputS_.IsPressed(INPUT_EVENT::RIGHT) || inputS_.IsPressed(INPUT_EVENT::LEFT))
	{
		//スティックサイズの取得
		LStickAngleSize_ = inputS_.GetKnockLStickSize(padNum_);

		//スティックの角度を求める
		stickDeg_ = inputS_.GetLStickDeg(padNum_);

		//ベクトルの計算
		VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y) };
		inputDir_ = VNorm(stickDir);
		isAct_.isRun = true;
	}

	//移動操作の時はカメラの角度を参照する
	if (isAct_.isRun && isCanMoveable_)
	{
		//カメラの角度を取得
		VECTOR cameraRot = camera_.lock()->GetAngles();
		Quaternion cameraQuaRot = camera_.lock()->GetQuaRotOutX();
		moveDir_ = cameraQuaRot.PosAxis(inputDir_);
	}

	//カードチャージ
	if (input_.IsPadBtnTrgDown(padNum_, CARD_CHARGE_BTN) || input_.IsTrgDown(CARD_CHARGE_KEY)) { isAct_.isCardCharge = true; }

	//カード使用
	if (input_.IsPadBtnTrgDown(padNum_, CARD_USE_BTN) || input_.IsTrgDown(CARD_USE_KEY)) { isAct_.isCardUse = true; }

	//カード移動
	if (IsCardLeft()) {isAct_.isCardMoveLeft = true;}
	if (IsCardRight()) {isAct_.isCardMoveRight = true;}

	//カード使用キー長押し(リロード用)
	if (input_.IsPadBtnNew(padNum_, CARD_USE_BTN) || input_.IsNew(CARD_USE_KEY)) { isAct_.isCardPushKeep = true; }

	//回避
	if (input_.IsPadBtnTrgDown(padNum_, DODGE_BTN) || input_.IsTrgDown(DODGE_KEY)) { isAct_.isDodge = true; }
}

void PlayerLogic::InputPad(void)
{
	actCntl_ = ACT_CNTL::NONE;

	//スティックの倒れ値が200以上だったら
	if (inputS_.IsPressed(INPUT_EVENT::UP) || inputS_.IsPressed(INPUT_EVENT::DOWN)
		|| inputS_.IsPressed(INPUT_EVENT::RIGHT) || inputS_.IsPressed(INPUT_EVENT::LEFT))
	{ 
		actCntl_ = ACT_CNTL::MOVE;

		//スティックの角度を求める
		stickDeg_ = inputS_.GetLStickDeg(padNum_);
	}

	VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y) };
	moveDir_ = VNorm(stickDir);
}

void PlayerLogic::KeyBoard(void)
{
}

void PlayerLogic::Pad(void)
{
}

bool PlayerLogic::IsCardRight(void)
{
	if (input_.IsKeyKeepPressed(CARD_MOVE_RIGHT_KEY, PUSH_KEEP_TIME)
		|| input_.IsBtnKeepPressed(padNum_, CARD_MOVE_RIGHT_BTN, PUSH_KEEP_TIME))
	{
		return true;
	}
	else if (input_.IsPadBtnTrgDown(padNum_, CARD_MOVE_RIGHT_BTN) || input_.IsTrgDown(CARD_MOVE_RIGHT_KEY))
	{
		return true;
	}
	return false;
}

bool PlayerLogic::IsCardLeft(void)
{
	if (input_.IsKeyKeepPressed(CARD_MOVE_LEFT_KEY, PUSH_KEEP_TIME)
		|| input_.IsBtnKeepPressed(padNum_, CARD_MOVE_LEFT_BTN, PUSH_KEEP_TIME))
	{
		return true;
	}
	else if (input_.IsPadBtnTrgDown(padNum_, CARD_MOVE_LEFT_BTN) || input_.IsTrgDown(CARD_MOVE_LEFT_KEY))
	{
		return true;
	}
	return false;
}
