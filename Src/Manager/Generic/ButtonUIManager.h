#pragma once
#include <unordered_map>
#include "../Template/Singleton.h"
#include "../Common/Vector2F.h"

class ButtonUIManager :
	public Singleton<ButtonUIManager>
{
	// シングルトン
	friend class Singleton<ButtonUIManager>;

public:

	//ボタンの種類
	enum class BTN_UI_TYPE
	{
		RSTICK_L,
		RSTICK_PUSH,
		RSTICK_R,
		RSTICK_UP,
		RSTICK_UP_DOWN,
		LSTICK_SIDE_VIEW,
		RSTICK_SIDE_VIEW,
		LSTICK_CIRCLE,
		RSTICK_CIRCLE,
		DUMMY,
		LSTICK_DOWN,
		LSTICK_LR,
		LSTICK_L,
		LSTICK_PUSH,
		LSTICK_R,
		LSTICK_UP,
		LSTICK_UP_DOWN,
		RSTICK_NEUTRAL,
		RSTICK_DOWN,
		RSTICK_LR,
		LSTICK_CIRCLE_NOPUSH,
		LTRIGGER_PUSH,
		LTRIGGER_NOPUSH,
		RBUTTON_PUSH,
		RBUTTON_NOPUSH,
		RSTICK_CIRCLE_PUSH,
		RSTICK_CIRCLE_NOPUSH,
		RTRIGGER_PUSH,
		RTRIGGER_NOPUSH,
		LSTICK_NEUTRAL,
		PADDLE_BOTTOM_R_NOPUSH,
		PADDLE_TOP_L_PUSH,
		PADDLE_TOP_L_NOPUSH,
		PADDLE_TOP_R_PUSH,
		PADDLE_TOP_R_NOPUSH,
		XBOX_BUTTON,
		XBOX_BUTTON_OUTLINE,
		LBUTTON_PUSH,
		LBUTTON_NOPUSH,
		LSTICK_CIRCLE_PUSH,
		DPAD_ROUND_R,
		DPAD_ROUND_UP,
		DPAD_ROUND_UP_DOWN,
		DPAD_UP,
		DPAD_UP_OUTLINE,
		DPAD_UP_DOWN,
		DPAD_UP_DOWN_OUTLINE,
		PADDLE_BUTTOM_L_PUSH,
		PADDLE_BUTTOM_L_NOPUSH,
		PADDLE_BUTTOM_R_PUSH,
		DPAD_LEFT,
		DPAD_LEFT_OUTLINE,
		DPAD_NEUTRAL_OUTLINE,
		DPAD_RIGHT,
		DPAD_RIGHT_OUTLINE,
		DPAD_ROUND_NEUTRAL,
		DPAD_ROUND_ALL,
		DPAD_ROUND_DOWN,
		DPAD_ROUND_LR,
		DPAD_ROUND_L,
		X_BUTTON_MONO_PUSH,
		X_BUTTON_MONO_NOPUSH,
		Y_BUTTON_MONO_PUSH,
		Y_BUTTON_MONO_NOPUSH,
		DPAD_NEUTRAL,
		DPAD_ALL,
		DPAD_DOWN,
		DPAD_DOWN_OUTLINE,
		DPAD_LR,
		DPAD_LR_OUTLINE,
		MENU_BUTTON_PUSH,
		MENU_BUTTON_NOPUSH,
		SHARE_BUTTON_PUSH,
		SHARE_BUTTON_NOPUSH,
		START_BUTTON_PUSH,
		START_BUTTON_ICON_PUSH,
		START_BUTTON_ICON_NOPUSH,
		START_BUTTON_NOPUSH,
		VIEW_BUTTON_PUSH,
		VIEW_BUTTON_NOPUSH,
		BACK_BUTTON_ICON_NOPUSH,
		BACK_BUTTON_NOPUSH,
		A_BUTTON_COL_PUSH,
		A_BUTTON_COL_NOPUSH,
		B_BUTTON_COL_PUSH,
		B_BUTTON_COL_NOPUSH,
		X_BUTTON_COL_PUSH,
		X_BUTTON_COL_NOPUSH,
		Y_BUTTON_COL_PUSH,
		Y_BUTTON_COL_NOPUSH,
		ADAPTIVE_CONTROLLER,
		XBOX360_CONTROLLER,
		XBOXONE_CONTROLLER,
		XBOXSERIES_CONTROLLER,
		A_BUTTON_MONO_PUSH,
		A_BUTTON_MONO_NOPUSH,
		B_BUTTON_MONO_PUSH,
		B_BUTTON_MONO_NOPUSH,
		BACK_BUTTON_PUSH,
		BACK_BUTTON_ICON_PUSH
	};

	//デフォルトのUIサイズ
	static constexpr float DEFAULT_UI_SIZE = 64.0f;

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief センター座標基準の描画
	/// @param _btnType ボタンの種類
	/// @param _centerPos 中心座標
	/// @param _size サイズ
	void DrawFromCenter(const BTN_UI_TYPE _btnType, const Vector2F _centerPos, const float _size = DEFAULT_UI_SIZE);

	/// @brief 左上基準の描画
	/// @param _btnType ボタンの種類
	/// @param _centerPos 左上座標
	/// @param _size サイズ
	void DrawFromLeftTop(const BTN_UI_TYPE _btnType, const Vector2F _leftTopPos, const float _size = DEFAULT_UI_SIZE);

private:

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ButtonUIManager(void);

	//コピー禁止
	ButtonUIManager(const ButtonUIManager& _copy) = delete;
	ButtonUIManager& operator=(const ButtonUIManager& _copy) = delete;

	// デストラクタも同様
	~ButtonUIManager(void)override = default;

	//ボタン
	int* buttonUI_;
};