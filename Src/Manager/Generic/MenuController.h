#pragma once
#include "../Common/Vector2.h"
#include "../Common/Easing.h"

class Easing;
class MenuController
{

public:

	//メニュー状態
	enum class MENU_STATE
	{
		DIRECTION,		//メニューが出てくる演出
		NORMAL,			//通常
	};

	//yes/noの選択肢
	enum class YES_NO
	{
		YES,
		NO,
		MAX
	};

	struct BTN_INFO
	{
		std::wstring btnStr;				//ボタンの文字
		Vector2 startPos;					//イージング前の座標
		Vector2 curPos = startPos;			//現在座標
		float directionEaseCnt=0.0f;					//イージング時間
		bool isEase = false;				//イージング中か
		bool isEndDirectEase = false;		//演出イージングが終わったか
	};

	/// @brief コンストラクタ
	/// @param  
	MenuController(void);

	/// @brief デストラクタ
	/// @param  
	~MenuController(void);

	/// @brief フォントのロード
	/// @param
	void LoadFont(const std::wstring _fontType,const int _size);

	/// @brief メニューの追加
	/// @param  
	void AddMenu(const int _arrayNum,const std::wstring _menu,const Vector2 _pos);

	/// @brief メニュー操作
	/// @param  
	void Update(void);

	/// @brief メニュー演出の更新
	/// @param _disSpawn 出てくる間隔時間
	/// @param _easeTime イージング時間
	/// @param _goalPosX 最終的なX座標
	/// @return true:演出終了	false:演出中
	void UpdateDirection(const float _disSpawn,const float _easeTime,const int _goalPosX);

	/// @brief 通常更新
	/// @param  
	void NormalUpdate(const Vector2 _localPos,const float _easeTime,const Easing::EASING_TYPE _easeType);

	/// @brief はい/いいえのメニュー演出の更新
	/// @param _localPos 
	/// @param _easeTime 
	/// @param _easeType 
	void SetYesNoUpdate(const bool _isYes);

	/// @brief すべてのメニューの演出イージングが終わっているかの取得
	/// @param  
	/// @return 
	const bool IsAllDirectEaseEnd(void) { return isAllDirectEaseEnd_; }

	/// @brief はいフラグの取得
	/// @param  
	/// @return 
	const bool GetIsYes(void) { return yesNoState_==YES_NO::YES; }

	/// @brief メニューの描画
	/// @param  
	void Draw(void);

	/// @brief はいいいえの描画
	/// @param _questionStr 質問内容
	void YesNoDraw(const std::wstring _questionStr);

	/// @brief 選択中のメニュー番号の加算
	/// @param  
	void AddSelectMenuNum(void);

	/// @brief 選択中のメニュー番号の加算
	/// @param  
	void SubSelectMenuNum(void);

	/// @brief 選択中のメニュー番号の取得
	/// @param  
	/// @return 
	const int GetSelectMenuNum(void) { return selectMenuNum_; }

private:

	//通常メニューのイージング時間
	static constexpr float SELECT_EASING_TIME = 0.5f;

	//ゲーム終了確認メニューの大きさ
	static constexpr int CHECK_EXIT_MENU_SIZE_X = 600;
	static constexpr int CHECK_EXIT_MENU_SIZE_Y = 200;

	//はい、いいえの文字間隔
	static constexpr int YES_NO_DISTANCE_X = 70;
	static constexpr int YES_NO_DISTANCE_Y = 100;

	//終了するかの文字の確認メニューとの間隔
	static constexpr int QUESTION_OFFSET = 30;

	//はい/いいえの選択肢の数
	static constexpr int YES_NO_NUM = 2;

	//はいいいえの文字列
	const std::wstring YES_STR = L"はい";
	const std::wstring NO_STR = L"いいえ";

	//デフォルトのフォントサイズ
	int defaultFontSize_;

	//フォントハンドル
	std::wstring defaultFontHandle_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//メニューのフォントハンドル
	int fontHandle_;

	//サイズイージングの時間カウント
	float sizeEaseCnt_;

	//演出でメニューが出てくるときの間隔カウント
	float disSpawnCnt_;

	//状態遷移
	std::unordered_map<MENU_STATE, std::function<void(void)>>changeState_;

	//はい/いいえの文字列
	std::wstring yesNoStrTable_[YES_NO_NUM];

	//更新関数
	std::function<void(void)>updateFunc_;

	//メニュー表
	std::unordered_map<int, BTN_INFO> menuList_;

	//サイズイージングのフォントハンドルのテーブル
	std::unordered_map<int, int> sizeEasingFontHandleTable_;

	//全体の演出が終わったか
	bool isAllDirectEaseEnd_;

	//選択中のメニュー
	int selectMenuNum_;

	//はい/いいえ状態
	YES_NO yesNoState_;

	//中央座標で描画
	void DrawFromCenter(const int _arrayNum, const unsigned int _color, const int _fontHandle);
};