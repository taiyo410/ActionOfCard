#include "FontManager.h"
#include <DxLib.h>
#include "../Application.h"

const std::wstring FontManager::FONT_DOT = L"ベストテンDOT";
const std::wstring FontManager::FONT_BOKUTATI = L"ぼくたちのゴシック";
const std::wstring FontManager::FONT_APRIL_GOTHIC = L"April Gothic one Regular";


FontManager::FontManager(void)
{
	//初期化
	for (int i = 0; i < static_cast<int>(FONT_TYPE::MAX); i++)
	{
		fontPath_[i] = FONT_INIT;
	}
}

void FontManager::Init(void)
{
	//フォント登録を行う
	// フォントパス初期化
	fontPath_[static_cast<int>(FONT_TYPE::DOT)] = FONT_DOT_PASS;
	fontPath_[static_cast<int>(FONT_TYPE::BOKUTATI)] = FONT_BOKUTATI_PASS;
	fontPath_[static_cast<int>(FONT_TYPE::APRIL_GOTHIC_ONE)] = FONT_APRIL_GOTHIC_PASS;

	// フォントをシステムに登録
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (AddFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}

void FontManager::Destroy(void)
{
	// フォント登録解除
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (RemoveFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}