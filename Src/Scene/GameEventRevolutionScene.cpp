#include "pch.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Renderer/PixelMaterial.h"
#include "Renderer/PixelRenderer.h"
#include "GameEventRevolutionScene.h"

GameEventRevolutionScene::GameEventRevolutionScene(void):
	fadeCnt_()
{
	//更新関数のセット
	updateFunc_ = [this]() {NormalUpdate(); };

	//描画関数のセット
	drawFunc_ = [this]() {NormalDraw(); };

	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

GameEventRevolutionScene::~GameEventRevolutionScene(void)
{
}

void GameEventRevolutionScene::Load(void)
{
	material_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::REVOLUTION_POSTEFF_PS);

	//メインスクリーン
	const int mainScreen = scnMng_.GetMainScreen();
	material_->AddTextureBuf(mainScreen);

	//マスク画像
	const int maskImg = resMng_.Load(ResourceManager::SRC::REVERSE_FADE_MASK).handleId_;
	material_->AddTextureBuf(maskImg);

	material_->AddConstBuf({ fadeCnt_ ,0.0f,0.0f,0.0f });

	renderer_ = std::make_shared<PixelRenderer>(*material_);
	renderer_->MakeScreenVertex();
}

void GameEventRevolutionScene::Init(void)
{
	//ポストエフェクトをSceneManager側へ
	scnMng_.SetPostEffect(renderer_);

	revolutionFadeFunc_ = [this]() {RevolutionFadeIn(); };

	fadeCnt_ = 0.0f;
}

void GameEventRevolutionScene::Release(void)
{
}

void GameEventRevolutionScene::NormalUpdate(void)
{
	revolutionFadeFunc_();

	material_->SetConstBuf(0, { fadeCnt_/FADE_TIME ,0.0f,0.0f,0.0f });
}

void GameEventRevolutionScene::NormalDraw(void)
{
	const int mainScreen = scnMng_.GetMainScreen();
	material_->SetTextureBuf(0, mainScreen);
}

void GameEventRevolutionScene::RevolutionFadeIn(void)
{
	//デバッグで革命をキーで起こす
	if (inputMng_.IsTrgDown(KEY_INPUT_1))
	{
		revolutionFadeFunc_ = [this]() {RevolutionFadeOut(); };
		return;
	}
	if (fadeCnt_ <= FADE_TIME)
	{
		fadeCnt_ += scnMng_.GetDeltaTime();
	}
}

void GameEventRevolutionScene::RevolutionFadeOut(void)
{
	if (fadeCnt_ > 0.0f)
	{
		fadeCnt_ -= scnMng_.GetDeltaTime();
	}
	else
	{
		scnMng_.DeletePostEffect(renderer_);
		scnMng_.PopScene();
		return;
	}
}

void GameEventRevolutionScene::OnSceneEnter(void)
{
	//ポストエフェクトをSceneManager側へ
	scnMng_.SetPostEffect(renderer_);
}
