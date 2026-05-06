#pragma once
#include <DxLib.h>
#include <map>				
#include <unordered_map>	
#include <string>			
#include <vector>			
#include <functional>
#include"InputManager.h"
#include "../../Template/Singleton.h"

//入力イベント
enum class INPUT_EVENT
{
	OK,		//決定
	CANCEL,	//キャンセル
	PAUSE,	//ポーズ
	RIGHT,	//右へ
	LEFT,	//左へ
	UP,		//上へ
	DOWN,	//下へ
	MAX
};
//入力イベントの最大数
static constexpr int INPUT_EVENT_MAX = static_cast<int>(INPUT_EVENT::MAX); 

//入力サイズ
static constexpr int INPUTABLE_STICK_SIZE = 50000;

/// 主変機器種別
enum class PEIPHERAL_TYPE
{
	KEYBOARD,	//キーボード
	GAMEPAD,	//ゲームパッド
	MOUSE,		//マウス
	X_ANALOG,	//XBOXアナログスティック
	MAX
};

//アナログ入力種類
enum class ANALOG_INPUT_TYPE
{
	NONE,
	L_UP,		//左スティックの上下
	L_DOWN,		//左スティックの上下
	L_RIGHT,	//左スティックの左右
	L_LEFT,		//左スティックの左右
	R_UP,		//右スティックの上下
	R_DOWN,		//右スティックの上下
	R_RIGHT,	//右スティックの左右
	R_LEFT,		//右スティックの左右
	L_TRIGGER,		//左トリガー
	R_TRIGGER,		//右トリガー
};

class InputManagerS 
	: public Singleton<InputManagerS>
{
	// シングルトン
	friend class Singleton<InputManagerS>;

public:	
	
	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 更新
	/// @param  
	void Update(void);
	
	/// @brief 解放
	/// @param  
	void Release(void);
	
	/// @brief 指定されたコードが押された瞬間か
	/// @param inputEvent 入力イベント
	/// @return 押された瞬間はtrue,そうでない場合fals
	bool IsTrgDown(INPUT_EVENT inputEvent) const;
	
	/// @brief 指定されたコードが押されているか
	/// @param inputEvent 入力イベント
	/// @return 押されてる場合true, そうでない場合false
	bool IsPressed(INPUT_EVENT inputEvent) const;
	
	/// @brief 対応表のリセット
	/// @param  
	void ResetTable(void);

	/// @brief Lスティックの角度を取得
	/// @param no コントローラー番号
	/// @return 
	float GetLStickDeg(InputManager::JOYPAD_NO no) const;

	/// @brief Rスティックの角度を取得
	/// @param no コントローラー番号
	/// @return 
	float GetRStickDeg(InputManager::JOYPAD_NO no) const;

	/// @brief 上を0.0度として角度を渡す
	/// @param no コントローラー番号
	/// @return 
	Vector2 GetKnockLStickSize(InputManager::JOYPAD_NO no) const;
	Vector2 GetKnockRStickSize(InputManager::JOYPAD_NO no) const;

private:

	//入力イベントの対応表
	struct InputState
	{
		PEIPHERAL_TYPE type;	//デバイスの種類
		uint32_t code;			//入力コード
	};

	//入力イベントと実際の入力の対応表
	using InputTable_t = std::unordered_map<INPUT_EVENT, std::vector<InputState>>;
	InputTable_t inputTable_;		//イベントと実際の入力の対応表	
	InputTable_t tmpInputTable_;	//一時的な入力テーブル

	//現在の状態と前の状態を比較するための変数
	InputState currentState_;		//現在の状態
	InputState lastState_;			//前の状態

	//現在の入力状態と前の入力状態を保持するための変数
	using InputData_t = std::unordered_map<INPUT_EVENT, bool>;
	InputData_t currentInput_;		//そのイベントに対応するボタンが押されてる状態か
	InputData_t lastInput_;			//そのイベントに対応するボタンが押されてた状態か(直前)

	//入力デバイスのリスト
	std::string inputListForDisplay_[INPUT_EVENT_MAX] = 
	{
		"OK",
		"Cancel",
		"Pause",
		"Right",
		"Left",
		"Up",
		"Down",
	};

	//アナログ入力の対応表
	using AnalogInputTable_t = std::unordered_map<ANALOG_INPUT_TYPE, std::function<bool(XINPUT_STATE&)>>;
	AnalogInputTable_t analogInputTable_;

	//コンストラクタ(シングルトンのためprivate)
	InputManagerS(void); 

	//コピー禁止
	InputManagerS(const InputManager& _copy) = delete;
	InputManagerS& operator=(const InputManager& _copy) = delete;

	//デストラクタ
	~InputManagerS(void)override = default;
};

