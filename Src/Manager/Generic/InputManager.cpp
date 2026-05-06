#include <DxLib.h>
#include "../Manager/Generic/SceneManager.h"
#include "InputManager.h"

InputManager::InputManager(void):
	mouseInput_(-1),
	infoEmpty_(),
	mousePos_(),
	padInfos_(),
	mouseInfos_(),
	mouseInfoEmpty_()
{
}

void InputManager::Init(void)
{

	// ゲームで使用したいキーを、
	// 事前にここで登録する
	Add(KEY_INPUT_SPACE);
	Add(KEY_INPUT_N);
	InputManager::GetInstance().Add(KEY_INPUT_Z);

	//プレイヤー操作関連
	//移動キー
	Add(KEY_INPUT_W);
	Add(KEY_INPUT_A);
	Add(KEY_INPUT_S);
	Add(KEY_INPUT_D);

	//カード使用
	Add(KEY_INPUT_F);
	Add(KEY_INPUT_RCONTROL);
	//カードチャージ
	Add(KEY_INPUT_R);

	//ドッジ(回避)
	Add(KEY_INPUT_LSHIFT);
	//カードセレクト
	Add(KEY_INPUT_E);
	Add(KEY_INPUT_Q);
	Add(KEY_INPUT_LCONTROL);
	//ポーズ
	Add(KEY_INPUT_P);

	//選択
	Add(KEY_INPUT_DOWN);
	Add(KEY_INPUT_UP);
	Add(KEY_INPUT_RIGHT);
	Add(KEY_INPUT_LEFT);

	//決定
	Add(KEY_INPUT_RETURN);

	//ポーズシーンを開く
	Add(KEY_INPUT_ESCAPE);

	InputManager::MouseInfo info;

	// 左クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_LEFT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

	// 右クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_RIGHT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

}

void InputManager::Update(void)
{
	// キーボード検知
	for (auto& p : keyInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
		p.second.UpdateKeepTime();
	}

	// マウス検知
	mouseInput_ = GetMouseInput();
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	for (auto& p : mouseInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = mouseInput_ == p.second.key;
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// パッド情報
	SetJPadInState(JOYPAD_NO::KEY_PAD1);
	SetJPadInState(JOYPAD_NO::PAD1);
	SetJPadInState(JOYPAD_NO::PAD2);
	SetJPadInState(JOYPAD_NO::PAD3);
	SetJPadInState(JOYPAD_NO::PAD4);

}

void InputManager::Release(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
}

void InputManager::Add(int key)
{
	InputManager::Info info = InputManager::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyCnt = 0.0f;
	info.keyKeeping = false;
	info.keyTrgUp = false;
	keyInfos_.emplace(key, info);
}

void InputManager::Clear(void)
{
	keyInfos_.clear();
}

bool InputManager::IsNew(int key) const
{
	return Find(key).keyNew;
}

bool InputManager::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

bool InputManager::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

Vector2 InputManager::GetMousePos(void) const
{
	return mousePos_;
}

int InputManager::GetMouse(void) const
{
	return mouseInput_;
}

bool InputManager::IsClickMouseLeft(void) const
{
	return mouseInput_ == MOUSE_INPUT_LEFT;
}

bool InputManager::IsClickMouseRight(void) const
{
	return mouseInput_ == MOUSE_INPUT_RIGHT;
}

bool InputManager::IsTrgMouseLeft(void) const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown;
}

bool InputManager::IsTrgMouseRight(void) const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown;
}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = keyInfos_.find(key);
	if (it != keyInfos_.end())
	{
		return it->second;
	}

	return infoEmpty_;

}

const InputManager::MouseInfo& InputManager::FindMouse(int key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

InputManager::JOYPAD_TYPE InputManager::GetJPadType(JOYPAD_NO no)
{
	return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no)));
}

DINPUT_JOYSTATE InputManager::GetJPadDInputState(JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
	return joyDInState_;
}

XINPUT_STATE InputManager::GetJPadXInputState(JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
	return joyXInState_;
}

const bool InputManager::IsKeyKeepPressed(const int _key, const float _time)
{
	auto it = keyInfos_.find(_key);
	if (it == keyInfos_.end())return infoEmpty_.keyKeeping;
	if (it->second.keyTrgDown)
	{
		it->second.keyCnt = _time;
		return false;
	}
	return it->second.keyKeeping;
}

const bool InputManager::IsBtnKeepPressed(JOYPAD_NO no, JOYPAD_BTN btn, const float _time)
{
	if (padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)])
	{
		padInfos_[static_cast<int>(no)].btnCnt[static_cast<int>(btn)] = _time;
		return false;
	}
	return padInfos_[static_cast<int>(no)].IsKeeping[static_cast<int>(btn)];
}

#ifdef _DEBUG
void InputManager::MoveObject(Vector2F& _pos, const float _spd)
{
	if (IsNew(KEY_INPUT_UP))
	{
		_pos.y -= _spd;
	}
	if (IsNew(KEY_INPUT_DOWN))
	{
		_pos.y += _spd;
	}
	if (IsNew(KEY_INPUT_LEFT))
	{
		_pos.x -= _spd;
	}
	if (IsNew(KEY_INPUT_RIGHT))
	{
		_pos.x += _spd;
	}

}
#endif // _DEBUG

void InputManager::SetJPadInState(JOYPAD_NO jpNo)
{
	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(JOYPAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{
		stateNow.ButtonsOld[i] = stateNow.ButtonsNew[i];
		stateNow.ButtonsNew[i] = stateNew.ButtonsNew[i];

		stateNow.IsOld[i] = stateNow.IsNew[i];
		stateNow.IsNew[i] = stateNow.ButtonsNew[i] > 0;

		stateNow.IsTrgDown[i] = stateNow.IsNew[i] && !stateNow.IsOld[i];
		stateNow.IsTrgUp[i] = !stateNow.IsNew[i] && stateNow.IsOld[i];

		//一定時間長押し情報更新
		UpdateKeepBtnTime(no, i);

		stateNow.AKeyLX = stateNew.AKeyLX;
		stateNow.AKeyLY = stateNew.AKeyLY;
		stateNow.AKeyRX = stateNew.AKeyRX;
		stateNow.AKeyRY = stateNew.AKeyRY;
	}
}

InputManager::JOYPAD_IN_STATE InputManager::GetJPadInputState(JOYPAD_NO no)
{

	JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

	auto type = GetJPadType(no);
	
	switch (type)
	{
	case InputManager::JOYPAD_TYPE::OTHER:
		break;
	case InputManager::JOYPAD_TYPE::XBOX_360:
	{
	}
		break;
	case InputManager::JOYPAD_TYPE::XBOX_ONE:
	{
		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//   Y
		// X   B
		//   A

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[0];// A

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = x.RightTrigger;// R_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = x.LeftTrigger; // L_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::R_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[5];// R_BUTTON

		idx = static_cast<int>(JOYPAD_BTN::L_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[4]; // L_BUTTON

		idx = static_cast<int>(JOYPAD_BTN::START_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[7];// START

		idx = static_cast<int>(JOYPAD_BTN::SELECT_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[6]; // SELECT

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_TOP);
		ret.ButtonsNew[idx] = x.Buttons[0]; // LEFTBUTTON_TOP

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_DOWN);
		ret.ButtonsNew[idx] = x.Buttons[1]; // LEFTBUTTON_DOWN

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_LEFT);
		ret.ButtonsNew[idx] = x.Buttons[2]; // LEFTBUTTON_LEFT

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = x.Buttons[3]; // LEFTBUTTON_RIGHT

		idx = static_cast<int>(JOYPAD_BTN::LEFT_STICK);
		ret.ButtonsNew[idx] = x.Buttons[6]; // 左スティック押し込み

		idx = static_cast<int>(JOYPAD_BTN::RIGHT_STICK);
		ret.ButtonsNew[idx] = x.Buttons[7]; // 右スティック押し込み

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// 右スティック
		ret.AKeyRX = d.Rx;
		ret.AKeyRY = d.Ry;
	}
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_4:
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SENSE:
	{
		auto d = GetJPadDInputState(no);
		int idx;

		//   △
		// □  〇
		//   ×

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// △

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[0];// □

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[2];// 〇

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[1];// ×

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;
		
		// 右スティック
		ret.AKeyRX = d.Z;
		ret.AKeyRY = d.Rz;
	}
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_L:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_R:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_PRO_CTRL:
		break;
	case InputManager::JOYPAD_TYPE::MAX:
		break;
	}

	return ret;

}

bool InputManager::IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)];
}

void InputManager::Info::UpdateKeepTime(void)
{
	if (!keyNew)
	{
		keyCnt = 0.0f;
		keyKeeping = false;
		return;
	}
	if (keyCnt > 0.0f)
	{
		keyKeeping = false;
		keyCnt -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		keyKeeping = true;
		keyCnt = 0.0f;
	}
}

void InputManager::UpdateKeepBtnTime(const int no, const int i)
{
	auto& stateNow = padInfos_[no];
	if (!stateNow.IsNew[i])
	{
		stateNow.btnCnt[i] = 0.0f;
		stateNow.IsKeeping[i] = false;
		return;
	}
	if (stateNow.btnCnt[i] > 0.0f)
	{
		stateNow.IsKeeping[i] = false;
		stateNow.btnCnt[i] -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		stateNow.IsKeeping[i] = true;
		stateNow.btnCnt[i] = 0.0f;
	}
}

