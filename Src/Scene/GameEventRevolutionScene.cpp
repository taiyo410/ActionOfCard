#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "GameEventRevolutionScene.h"

GameEventRevolutionScene::GameEventRevolutionScene(void)
{
	//更新関数のセット
	updateFunc_ = [this]() {NormalUpdate(); };

	//描画関数のセット
	drawFunc_ = [this]() {NormalDraw(); };

	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	material_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::REVOLUTION_POST_PS);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

GameEventRevolutionScene::~GameEventRevolutionScene(void)
{
}

void GameEventRevolutionScene::Load(void)
{
}

void GameEventRevolutionScene::Init(void)
{
	//material_->AddTextureBuf()
}

void GameEventRevolutionScene::Release(void)
{
}

void GameEventRevolutionScene::NormalUpdate(void)
{

}

void GameEventRevolutionScene::NormalDraw(void)
{
	renderer_->Draw();
}
