#pragma once
#include <DxLib.h>
#include <memory>
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Utility/Utility3D.h"
#include "../Base/LogicBase.h"
//#define DEBUG_ON
class PlayerBase;
class Command;

class PlayerLogic:public LogicBase
{

public:

    //デルタタイム
    static constexpr float DELTA_TIME = 1.0f / 60.0f;

    enum class CNTL
    {
        NONE,
        KEYBOARD,
        PAD
    };

    /// @brief コンストラクタ
    /// @param _padNum パッド番号
    /// @param _cntl 操作タイプ
    PlayerLogic(Transform& _myTrans,bool& _isCanMoveAble,InputManager::JOYPAD_NO _padNum, InputManager::CONTROLL_TYPE _cntl);

    /// @brief デストラクタ
    /// @param  
    ~PlayerLogic(void)override;

	/// @brief 変更時の初期化
	/// @param  
	void Init(void)override;
    
    /// @brief 更新
    /// @param  
    void Update(void)override;

#ifdef _DEBUG
#endif // _DEBUG_ON

private:

    //各アクション操作ボタン
    using JOYPAD_BTN = InputManager::JOYPAD_BTN;

    //移動
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;      //前
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;       //左
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;       //後ろ
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;      //右

    //カード選択
    //右
    static constexpr int CARD_MOVE_RIGHT_KEY = KEY_INPUT_E;
    static constexpr JOYPAD_BTN CARD_MOVE_RIGHT_BTN = JOYPAD_BTN::R_BUTTON;
    static constexpr int CARD_MOVE_LEFT_KEY = KEY_INPUT_Q;
    static constexpr JOYPAD_BTN CARD_MOVE_LEFT_BTN = JOYPAD_BTN::L_BUTTON;
    //カード使用キー
    static constexpr int CARD_USE_KEY = KEY_INPUT_F;
    static constexpr JOYPAD_BTN CARD_USE_BTN = JOYPAD_BTN::RIGHTBUTTON_RIGHT;
    static constexpr float PUSH_KEEP_TIME = 0.5f;

    //カードチャージ
    static constexpr int CARD_CHARGE_KEY = KEY_INPUT_R;
    static constexpr JOYPAD_BTN CARD_CHARGE_BTN = JOYPAD_BTN::RIGHTBUTTON_TOP;

    //移動判定になるスティックの大きさ
    static constexpr int STICK_MOVE_SIZE_MIN = 200;

    //パンチ
    static constexpr int DODGE_KEY = KEY_INPUT_LSHIFT;
    static constexpr InputManager::JOYPAD_BTN DODGE_BTN = InputManager::JOYPAD_BTN::RIGHTBUTTON_LEFT;

    //ジャンプ
    static constexpr int JUMP_KEY = KEY_INPUT_SPACE;
    static constexpr InputManager::JOYPAD_BTN JUMP_BTN = InputManager::JOYPAD_BTN::RIGHTBUTTON_DOWN;

    //角度関連
    static constexpr float FLONT_DEG = 0.0f;
    static constexpr float LEFT_DEG = 270.0f;
    static constexpr float BACK_DEG = 180.0f;
    static constexpr float RIGHT_DEG = 90.0f;

    //入力デバイス別の更新
    void InputKeyBoard(void);   //キーボード
    void InputAll(void);        //両対応
    void InputPad(void);        //パッド

    void KeyBoard(void);
    void Pad(void);

    //カードリボルバーの右方向の入力フラグを取得
    bool IsCardRight(void);

    //カードリボルバーの左方向の入力フラグを取得
    bool IsCardLeft(void);

    //スティック
    float leftStickX_;          //スティックの角度X
    float leftStickY_;          //スティックの角度Y
    bool& isCanMoveable_;       //移動可能か

    //スティックの角度(0～1000を返す)
    Vector2 LStickAngleSize_;
    float stickDeg_;            //パッドのスティックの角度

    //カメラ
    std::weak_ptr<Camera>camera_;

    InputManager::CONTROLL_TYPE cntl_;              //入力デバイス
    InputManager::JOYPAD_NO padNum_;                //パッド番号

    //入力情報
    InputManager& input_;

    //スティックの入力情報
    InputManagerS& inputS_;

    //入力更新
    std::map<InputManager::CONTROLL_TYPE, std::function<void(void)>>inputUpdates_;
};

