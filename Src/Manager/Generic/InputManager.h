#pragma once
#include <map>
#include <Dxlib.h>
#include "../../Template/Singleton.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"

class InputManager 
	: public Singleton<InputManager>
{
	// シングルトン
	friend class Singleton<InputManager>;

public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キー入力
	};

	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
	enum class JOYPAD_TYPE
	{
		OTHER = 0,
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		SWITCH_JOY_CON_L,
		SWITCH_JOY_CON_R,
		SWITCH_PRO_CTRL,
		MAX
	};

	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		RIGHTBUTTON_LEFT = 0,	// X
		RIGHTBUTTON_RIGHT,		// B
		RIGHTBUTTON_TOP,		// Y
		RIGHTBUTTON_DOWN,		// A
		R_TRIGGER,				// R_TRIGGER
		L_TRIGGER,				// L_TRIGGER
		R_BUTTON,				// R_BUTTON
		L_BUTTON,				// L_BUTTON
		START_BUTTON,			// START_BUTTON
		SELECT_BUTTON,			// SELECT_BUTTON
		LEFTBUTTON_TOP,			// 上
		LEFTBUTTON_DOWN,		// 下
		LEFTBUTTON_LEFT,		// 左
		LEFTBUTTON_RIGHT,		// 右
		LEFT_STICK,				// 左スティック押し込み
		RIGHT_STICK,			// 右スティック押し込み
		MAX
	};

	// ゲームコントローラーの入力情報
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsBtnKeep[static_cast<int>(JOYPAD_BTN::MAX)];
		float btnCnt[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsKeeping[static_cast<int>(JOYPAD_BTN::MAX)];
		int AKeyLX;
		int AKeyLY;
		int AKeyRX;
		int AKeyRY;
	};

	//コントローラーデバイス
	enum class CONTROLL_TYPE
	{
		KEY_BOARD		//キーボード
		,CONTROLLER		//コントローラー
		,ALL			//すべての入力を受け付ける
	};

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief  リソースの破棄
	/// @param  
	void Release(void);

	/// @brief 判定を行うキーを追加
	/// @param key 
	void Add(int key);

	/// @brief 判定を行うキーをクリア
	/// @param  
	void Clear(void);

	/// @brief キーの押下判定
	/// @param key 
	/// @return 
	bool IsNew(int key) const;

	/// @brief  キーの押下判定(押しっぱなしはNG)
	/// @param key キー
	/// @return 
	bool IsTrgDown(int key) const;

	/// @brief キーを離した時の判定
	/// @param key キー
	/// @return 
	bool IsTrgUp(int key) const;

	/// @brief  マウス座標の取得
	/// @param  
	/// @return マウス座標
	Vector2 GetMousePos(void) const;

	/// @brief マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	/// @param  
	/// @return マウスのクリック状態
	int GetMouse(void) const;

	/// @brief マウスが左クリックされたか
	/// @param  
	/// @return 
	bool IsClickMouseLeft(void) const;

	/// @brief マウスが右クリックされたか
	/// @param  
	/// @return 
	bool IsClickMouseRight(void) const;

	/// @brief マウスが左クリックされたか(押しっぱなしはNG)
	/// @param  
	/// @return 
	bool IsTrgMouseLeft(void) const;

	/// @brief  マウスが右クリックされたか(押しっぱなしはNG)
	/// @param  
	/// @return 
	bool IsTrgMouseRight(void) const;

	/// @brief コントローラの入力情報を取得する
	/// @param no 
	/// @return 
	JOYPAD_IN_STATE GetJPadInputState(JOYPAD_NO no);

	/// @brief ボタンが押された
	/// @param no コントローラー番号
	/// @param btn ボタン
	/// @return 
	bool IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const;

	/// @brief ボタンが押された(押しっぱなしNG)
	/// @param no コントローラー番号
	/// @param btn ボタン
	/// @return 
	bool IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const;

	/// @brief ボタンが離された
	/// @param no コントローラー番号
	/// @param btn ボタン
	/// @return 
	bool IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const;

	/// @brief  コントローラの入力情報を取得する
	/// @param no コントローラー番号
	/// @return 
	DINPUT_JOYSTATE GetJPadDInputState(JOYPAD_NO no);

	/// @brief  コントローラ(XBOX)の入力情報を取得する
	/// @param no コントローラー番号
	/// @return 
	XINPUT_STATE GetJPadXInputState(JOYPAD_NO no);

	/// @brief 一定時間以上押したかどうか
	/// @param _key キー
	/// @param _time キー入力時間
	/// @return 
	const bool IsKeyKeepPressed(const int _key, const float _time);

	/// @brief 一定時間以上押したかどうか
	/// @param _key キー
	/// @param _time キー入力時間
	/// @return 
	const bool IsBtnKeepPressed(JOYPAD_NO no, JOYPAD_BTN btn, const float _time);

#ifdef _DEBUG

	/// @brief オブジェクトを動かす(デバッグ用)
	/// @param _pos 
	void MoveObject(Vector2F& _pos, const float _spd = 1);
#endif // _DEBUG

private:

	// キー情報
	struct Info
	{
		int key;					// キーID
		bool keyOld;				// 1フレーム前の押下状態
		bool keyNew;				// 現フレームの押下状態
		float keyCnt;				// 何秒間キーが押されたか
		float keyTime;				// キー長押し時間格納
		bool keyTrgDown;			// 現フレームでボタンが押されたか
		bool keyTrgUp;				// 現フレームでボタンが離されたか
		bool keyKeeping;			// キー長押し中か(キー長押し時間指定)
		void UpdateKeepTime(void);	//一定時間キー長押し判定
	};

	// マウス
	struct MouseInfo
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// キー情報
	std::map<int, InputManager::Info> keyInfos_;
	InputManager::Info infoEmpty_;

	// マウス情報
	std::map<int, InputManager::MouseInfo> mouseInfos_;
	InputManager::MouseInfo mouseInfoEmpty_;

	// マウスカーソルの位置
	Vector2 mousePos_;
	
	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[5];
	
	//コンストラクタ(シングルトンのためprivate)
	InputManager(void);

	//コピー禁止
	InputManager(const InputManager& _copy) = delete;
	InputManager& operator=(InputManager& _copy) = delete;

	//デストラクタ
	~InputManager(void)override = default;

	// 配列の中からキー情報を取得する
	const InputManager::Info& Find(int key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(int key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	// コントローラの入力情報を更新する
	void SetJPadInState(JOYPAD_NO jpNo);

	// キーの長押し時間を更新する
	void UpdateKeepBtnTime(const int no,const int i);
};