#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Application.h"
#include "../Common/Easing.h"
#include "MenuController.h"

MenuController::MenuController(void) :
	selectMenuNum_(0),
	disSpawnCnt_(0.0f),
	sizeEaseCnt_(0.0f),
	isAllDirectEaseEnd_(false),
	yesNoStrTable_{ YES_STR, NO_STR },
	yesNoState_(YES_NO::NO),
	defaultFontSize_(),
	defaultFontHandle_(L""),
	fontHandle_()
{
	easing_ = std::make_unique<Easing>();
}

MenuController::~MenuController(void)
{
}

void MenuController::LoadFont(const std::wstring _fontType, const int _size)
{
	defaultFontHandle_ = _fontType;
	defaultFontSize_ = _size;
	if (_fontType.empty())
	{
		fontHandle_ = DX_DEFAULT_FONT_HANDLE;
	}
	else
	{
		fontHandle_ = CreateFontToHandle(_fontType.c_str(), _size, 0);
		sizeEasingFontHandleTable_[_size] = fontHandle_;
	}
}

void MenuController::AddMenu(const int _arrayNum, const std::wstring _menu,const Vector2 _pos)
{
	//情報の代入
	BTN_INFO info;
	info.btnStr = _menu;
	info.startPos = _pos;
	info.curPos = _pos;

	menuList_.emplace(_arrayNum, info);
}

void MenuController::Update(void)
{
}

void MenuController::UpdateDirection(const float _disSpawn, const float _easeTime, const int _goalPosX)
{
	//すべてのメニューが演出イージングが終わっているかをチェック
	isAllDirectEaseEnd_ = std::all_of(menuList_.begin(), menuList_.end(), [](const auto& menu) { return menu.second.isEndDirectEase; });

	//すべてのメニューが演出イージングが終わっている場合は、これ以上更新しない
	if (isAllDirectEaseEnd_)return;

	//間隔カウントが一定時間を超えたらイージング開始
	for (auto& menu : menuList_)
	{
		//イージング中またはイージングが終わっているものはスキップ
		if (menu.second.isEase||menu.second.isEndDirectEase)continue;
		if (disSpawnCnt_ > _disSpawn)
		{
			disSpawnCnt_ = 0.0f;
			menu.second.isEase = true;
			break;
		}
	}

	//イージング中のメニューの座標を更新
	for (auto& menu : menuList_)
	{
		if (menu.second.isEase)
		{
			Vector2 goalPos = { _goalPosX,menu.second.curPos.y };
			menu.second.curPos = easing_->EaseFunc(menu.second.startPos, goalPos, menu.second.directionEaseCnt / _easeTime, Easing::EASING_TYPE::OUT_BACK);
			menu.second.directionEaseCnt += SceneManager::GetInstance().GetDeltaTime();
			if (menu.second.directionEaseCnt >= _easeTime)
			{
				menu.second.isEase = false;
				menu.second.isEndDirectEase = true;
				menu.second.curPos = goalPos;
				menu.second.startPos = goalPos;
			}
		}
	}
	if (disSpawnCnt_ > _disSpawn)
	{
		disSpawnCnt_ = 0.0f;
	}
	disSpawnCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void MenuController::NormalUpdate(const Vector2 _localPos, const float _easeTime, const Easing::EASING_TYPE _easeType)
{
	for (auto& menu : menuList_)
	{
		//選択中のメニュー左右に動かす
		if (menu.first == selectMenuNum_)
		{
			if (sizeEaseCnt_ >= _easeTime)
			{
				sizeEaseCnt_ = 0.0f;
			}
			sizeEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();
			menu.second.curPos = easing_->EaseFunc(menu.second.startPos, menu.second.startPos + _localPos, sizeEaseCnt_ / _easeTime, _easeType);
		}
		else
		{
			menu.second.curPos = menu.second.startPos;
		}
	}
}

void MenuController::SetYesNoUpdate(const bool _isYes)
{	
	_isYes ? yesNoState_ = YES_NO::YES : yesNoState_ = YES_NO::NO;
}


void MenuController::Draw(void)
{ 
	unsigned int color = UtilityCommon::WHITE;
	for (auto& menu : menuList_)
	{
		//選択中のメニューはサイズイージングして赤色で描画
		if (menu.first == selectMenuNum_)
		{
			color = UtilityCommon::RED;
		}
		else
		{
			color = UtilityCommon::WHITE;
		}
		DrawFormatStringToHandle(
			menu.second.curPos.x, menu.second.curPos.y, color, fontHandle_, menu.second.btnStr.c_str());
	}
}

void MenuController::YesNoDraw(const std::wstring _questionStr)
{
	const Vector2 startPos = { Application::SCREEN_HALF_X - (CHECK_EXIT_MENU_SIZE_X / 2)
						,Application::SCREEN_HALF_Y - (CHECK_EXIT_MENU_SIZE_Y / 2) };
	const Vector2 endPos = { Application::SCREEN_HALF_X + (CHECK_EXIT_MENU_SIZE_X / 2),
							Application::SCREEN_HALF_Y + (CHECK_EXIT_MENU_SIZE_Y / 2) };

	//メニュ背景
	DrawBox(startPos.x,
		startPos.y,
		endPos.x,
		endPos.y,
		0x00ff00,
		true
	);

	//質問の文字列描画
	DrawFormatStringToHandle(
		startPos.x + QUESTION_OFFSET,
		startPos.y + QUESTION_OFFSET,
		UtilityCommon::BLACK,
		fontHandle_,
		_questionStr.c_str()
	);

	int size = static_cast<int>(yesNoStrTable_->size());
	for (int i = 0; i < size; i++)
	{
		unsigned int btnCol = UtilityCommon::WHITE;
		//選択中の文字は赤色で描画
		yesNoState_ == static_cast<YES_NO>(i) ? btnCol = UtilityCommon::RED : btnCol = UtilityCommon::WHITE;

		DrawFormatStringToHandle(
			startPos.x + YES_NO_DISTANCE_X + i * YES_NO_DISTANCE_Y,
			startPos.y + YES_NO_DISTANCE_Y,
			btnCol,
			fontHandle_,
			yesNoStrTable_[i].c_str()
		);
	};
}

void MenuController::AddSelectMenuNum(void)
{
	selectMenuNum_++;
	if (selectMenuNum_ >= static_cast<int>(menuList_.size()))
	{
		selectMenuNum_ = 0;
	}
}

void MenuController::SubSelectMenuNum(void)
{
	selectMenuNum_--;
	if (selectMenuNum_ < 0)
	{
		selectMenuNum_ = static_cast<int>(menuList_.size()) - 1;
	}
}

void MenuController::DrawFromCenter(const int _arrayNum, const unsigned int _color, const int _fontHandle)
{
	Vector2 strPos = menuList_[_arrayNum].curPos;
	int w, h;
	GetDrawStringSizeToHandle(
		&w,
		&h,
		NULL,
		menuList_[_arrayNum].btnStr.c_str()
		,static_cast<int>(wcslen(menuList_[_arrayNum].btnStr.c_str()))
		,_fontHandle );

	//中心基準の座標に変換
	strPos.x += static_cast<int>(w * 0.5f);
	strPos.y += static_cast<int>(h * 0.5f);

	// 中心基準描画
	DrawRotaStringFToHandle(
		static_cast<float>(strPos.x),
		static_cast<float>(strPos.y),
		1.0,
		1.0,
		w * 0.5,
		h * 0.5,   
		0.0f,
		_color,
		_fontHandle,
		0,
		FALSE,
		menuList_[_arrayNum].btnStr.c_str()
	);
}

