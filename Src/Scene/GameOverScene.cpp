#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void):
	soundMng_(SoundManager::GetInstance())
{
	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };

	//描画関数のセット
	drawFunc_ = [this]() {LoadingDraw(); };

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FIXED_POINT);
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	imgGameOver_ = resMng_.GetInstance().Load(ResourceManager::SRC::GAME_OVER_IMG).handleId_;

	//BGMロード
	resMng_.Load(ResourceManager::SRC::GAME_OVER_BGM);

}

void GameOverScene::Init(void)
{
	//BGM再生
	soundMng_.GetInstance().Play(ResourceManager::SRC::GAME_OVER_BGM, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(BGM_VOL, SoundManager::TYPE::BGM);
}

void GameOverScene::Release(void)
{
	soundMng_.Stop(ResourceManager::SRC::GAME_OVER_BGM);
}

void GameOverScene::NormalUpdate(void)
{
	if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::NormalDraw(void)
{
	//ゲームオーバー画像
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameOver_, true);

	//Bボタン、スペースキーでタイトルに戻るの描画
	UtilityDraw::DrawStringCenter(
		Application::SCREEN_HALF_X,
		static_cast<int>(BACK_TITLE_STRING_POS_Y),
		BACK_TITLE_SCENE_STR,
		UtilityCommon::WHITE,
		buttonFontHandle_
	);
}

void GameOverScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}
