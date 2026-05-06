#include "InputManagerS.h"

void InputManagerS::Init(void)
{
	//入力イベントの初期化
	ResetTable();

	//アナログ入力の対応表を初期化
	analogInputTable_[ANALOG_INPUT_TYPE::L_UP] = [](const XINPUT_STATE& state) 
		{
		return state.ThumbLY > INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::L_DOWN] = [](const XINPUT_STATE& state) 
		{
		return state.ThumbLY < -INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::L_RIGHT] = [](const XINPUT_STATE& state)
		{
		return state.ThumbLX > INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::L_LEFT] = [](const XINPUT_STATE& state)
		{
		return state.ThumbLX < -INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::R_UP] = [](const XINPUT_STATE& state)
		{
		return state.ThumbRY > INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::R_DOWN] = [](const XINPUT_STATE& state) 
		{
		return state.ThumbRY < -INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::R_RIGHT] = [](const XINPUT_STATE& state)
		{
		return state.ThumbRX > INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::R_LEFT] = [](const XINPUT_STATE& state) 
		{
		return state.ThumbRX < -INPUTABLE_STICK_SIZE;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::L_TRIGGER] = [](const XINPUT_STATE& state) 
		{
		return state.LeftTrigger > 10000;
		};
	analogInputTable_[ANALOG_INPUT_TYPE::R_TRIGGER] = [](const XINPUT_STATE& state) 
		{
		return state.RightTrigger > 10000;
		};

	for (const auto& keyValue : inputTable_)
	{
		currentInput_[keyValue.first] = false;
	}
	lastInput_ = currentInput_;
}

void InputManagerS::Update(void)
{
	//押したか押してないか記録する
	lastInput_ = currentInput_;	//前のプッシュ情報を記録する

	//キーボード情報
	char keyState[256] = {};
	GetHitKeyStateAll(keyState);

	//パッド情報
	int padState = GetJoypadInputState(DX_INPUT_PAD1);

	//マウス情報
	int mouseState = GetMouseInput();

	//アナログ入力情報
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);

	//テーブルの行を回す
	for (const auto& keyValue : inputTable_)
	{
		//特定のキーの入力情報を取得
		for (auto input : keyValue.second)
		{
			bool pressed = false;	//押してない状態で初期化
			if (input.type == PEIPHERAL_TYPE::KEYBOARD)
			{
				pressed = keyState[input.code];
			}
			else if (input.type == PEIPHERAL_TYPE::GAMEPAD)
			{
				pressed = padState & input.code;
			}
			else if (input.type == PEIPHERAL_TYPE::MOUSE)
			{
				pressed = mouseState & input.code;
			}
			else if (input.type == PEIPHERAL_TYPE::X_ANALOG)
			{
				auto analogType = static_cast<ANALOG_INPUT_TYPE>(input.code);
				if (analogInputTable_.contains(analogType))
				{
					pressed = analogInputTable_.at(analogType)(xinputState);
				}
			}
			currentInput_[keyValue.first] = pressed;
			if (pressed)
			{
				break;
			}
		}
	}
}

void InputManagerS::Release(void)
{
}

bool InputManagerS::IsTrgDown(INPUT_EVENT inputEvent) const
{
	if (!currentInput_.contains(inputEvent))
	{
		return false;
	}
	return currentInput_.at(inputEvent) && !lastInput_.at(inputEvent);
}

bool InputManagerS::IsPressed(INPUT_EVENT inputEvent) const
{
	return currentInput_.at(inputEvent);
}

void InputManagerS::ResetTable(void)
{
	inputTable_ = 
	{
		{INPUT_EVENT::OK,{
			{PEIPHERAL_TYPE::KEYBOARD, KEY_INPUT_SPACE},
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_B},//Bボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::NONE}
	}},
		{INPUT_EVENT::CANCEL,{
			{PEIPHERAL_TYPE::KEYBOARD, KEY_INPUT_RETURN},
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_A},	//Aボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::NONE},	//右トリガー
	}},
		{INPUT_EVENT::PAUSE,{
			{PEIPHERAL_TYPE::KEYBOARD, KEY_INPUT_P},
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_R},	//スタートボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::NONE}
	}},
		{INPUT_EVENT::RIGHT,{
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_RIGHT},	//十字右ボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::L_RIGHT}
	}},
		{INPUT_EVENT::LEFT,{
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_LEFT},	//十字左ボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::L_LEFT},
	}},
		{INPUT_EVENT::UP,{
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_UP},	//十字上ボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::L_UP}
	}},
		{INPUT_EVENT::DOWN,{
			{PEIPHERAL_TYPE::GAMEPAD, PAD_INPUT_DOWN},	//十字下ボタン
			{PEIPHERAL_TYPE::X_ANALOG, (int)ANALOG_INPUT_TYPE::L_DOWN},
	}},
	};
}

float InputManagerS::GetLStickDeg(InputManager::JOYPAD_NO no) const
{
	float deg = 0.0f;
	Vector2 knockSize = GetKnockLStickSize(no);
	if (knockSize.x == 0.0f && knockSize.y == 0.0f)
	{
		return deg;
	}
	auto rad = std::atan2(knockSize.y, knockSize.x);
	deg = static_cast<float>(rad)* (180.0f / DX_PI_F);
	deg += 90.0f;
	deg = deg < 0 ? deg + 360 : deg;
	return deg;
}

float InputManagerS::GetRStickDeg(InputManager::JOYPAD_NO no) const
{
	float deg = 0.0f;
	Vector2 knockSize = GetKnockRStickSize(no);
	if (knockSize.x == 0.0f && knockSize.y == 0.0f)
	{
		return deg;
	}
	auto rad = std::atan2(knockSize.y, knockSize.x);
	deg = static_cast<float>(rad) * (180.0f / DX_PI_F);
	deg += 90.0f;
	deg = deg < 0.0f ? deg + 360.0f : deg;
	return deg;
}

Vector2 InputManagerS::GetKnockLStickSize(InputManager::JOYPAD_NO no) const
{
	auto padInfo = InputManager::GetInstance().GetJPadInputState(no);
	return Vector2(padInfo.AKeyLX, padInfo.AKeyLY);
}

Vector2 InputManagerS::GetKnockRStickSize(InputManager::JOYPAD_NO no) const
{
	auto padInfo = InputManager::GetInstance().GetJPadInputState(no);
	return Vector2(padInfo.AKeyRX, padInfo.AKeyRY);
}

InputManagerS::InputManagerS()
{
}
