#include "PauseScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Utility/UtilityCommon.h"

PauseScene::PauseScene(void):
	pauseFont_(UtilityCommon::INITIAL_HANDLE),
	selectIndex_()
{
	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };

	//描画関数のセット
	drawFunc_ = [this]() {LoadingDraw(); };
	
	//リストごとに処理を分ける
	listFuncTable_ =
	{
		//ポーズを解除して前のシーンに戻る
		{LIST::RESUME,[this]() {scnMng_.PopScene(); }},	
		//タイトルシーンに戻る
		{LIST::TITLE,[this](){scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);}},
		//ゲームを終了する
		{LIST::GAME_END,[this](){Application::GetInstance().IsGameEnd(); }}

	};
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Load(void)
{
	pauseFont_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, FONT_THICK);
}

void PauseScene::Init(void)
{	
	
}
void PauseScene::Release(void)
{

}
void PauseScene::NormalUpdate(void)
{
	if (inputMng_.IsTrgDown(KEY_INPUT_ESCAPE))
	{
		//シーンを戻す
		scnMng_.PopScene();
		return;
	}
	else if (inputMng_.IsTrgDown(KEY_INPUT_DOWN)||inputMngS_.IsTrgDown(INPUT_EVENT::DOWN))
	{
		selectIndex_ = (selectIndex_ - 1 + LIST_MAX) % LIST_MAX;
	}
	else if (inputMng_.IsTrgDown(KEY_INPUT_UP) || inputMngS_.IsTrgDown(INPUT_EVENT::UP))
	{
		selectIndex_ = (selectIndex_ + 1) % LIST_MAX;
	}
	else if (inputMng_.IsTrgDown(KEY_INPUT_RETURN)||inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		listFuncTable_[static_cast<LIST>(selectIndex_)]();
		return;
	}
}

void PauseScene::NormalDraw(void)
{
	static constexpr int MARGINT = 50;
	static constexpr int OFFSET_Y = 100;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PAUSE_ALPHA);
	DrawBox(
		MARGINT,
		MARGINT,
		Application::SCREEN_SIZE_X- MARGINT,
		Application::SCREEN_SIZE_Y- MARGINT,
		UtilityCommon::WHITE,
		true);	

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < LIST_MAX; ++i)
	{
		//カラーを設定
		int color = UtilityCommon::BLUE;
		if (i == selectIndex_)
		{
			color = UtilityCommon::RED; // 選択中は赤色
		}

		//座標位置を設定
		int posX = static_cast<int>(Application::SCREEN_HALF_X - pauseList_[i].length() * FONT_SIZE / 2);
		int posY = Application::SCREEN_HALF_Y - OFFSET_Y * i;

		//文字列を描画
		DrawFormatStringToHandle(
			posX,
			posY,
			color,
			pauseFont_,
			pauseList_[i].c_str());
	}
}

void PauseScene::OnSceneEnter(void)
{
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}
