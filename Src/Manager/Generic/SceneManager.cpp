#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/Generic/DataBank.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/ButtonUIManager.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Scene/GameEventRevolutionScene.h"
#include "Scene/GameClearScene.h"
#include "Scene/GameOverScene.h"
#include "Renderer/PixelRenderer.h"
#include "SceneManager.h"
#include "Camera.h"

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// カメラ
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	isSceneChanging_ = true;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	mainScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		true);

	postEffScreen_= MakeScreen(
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		true);

	//ウィンドウがアクティブ状態でなくとも処理を行う
	SetAlwaysRunFlag(true);

	//データバンクを生成
	DataBank::CreateInstance();

	//ボタンUIマネージャ生成
	ButtonUIManager::CreateInstance();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	
	 //ライトの設定
	ChangeLightTypeDir(LIGHT_DIR);

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(FOG_COLOR_R, FOG_COLOR_G, FOG_COLOR_B);
	SetFogStartEnd(FOG_START,FOG_END);

	//シャドウマップのテクスチャを作成
	MakeShadowMapTexture();
}

void SceneManager::Update(void)
{
	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
	totalTime_ += deltaTime_;

	fader_->Update();
	if (isSceneChanging_)
	{
		SceneChangeFade();
	}
	Fade();
	// カメラ更新
	camera_->Update();

	//シーンごとの更新
	scenes_.back()->Update();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void SceneManager::Draw(void)
{
	MainScreenDraw();

	PostEffectScreen();

	SetDrawScreen(DX_SCREEN_BACK);

	// メインスクリーンを画面に描画する
	DrawGraph(0, 0, postEffScreen_, false);
}

void SceneManager::CreateScene(const SCENE_ID _sceneId)
{
	const std::shared_ptr<SceneBase> scenePtr_ = createScenePtr_[_sceneId]();

;	if (scenes_.empty())
	{
		scenes_.push_back(scenePtr_);
	}
	else
	{
		scenes_.front() = scenePtr_;
	}

	//データのロード
	scenes_.front()->Load();
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	scene->Init();
	scenes_.push_back(scene);
}

void SceneManager::PopScene()
{
	if (scenes_.size() >= 1)
	{
		scenes_.pop_back();
	}
}

void SceneManager::Release(void)
{
	DataBank::Destroy();
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	//シーン遷移中は処理抜け(無限にシーン遷移を防ぐため)
	if (isSceneChanging_)return;

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
  	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
	isEndFade_ = false;
}

void SceneManager::StartFadeIn(void)
{
	//フェードを明ける
	fader_->SetFade(Fader::STATE::FADE_IN);
}

void SceneManager::StartFadeOut(void)
{
	//フェードを明ける
	fader_->SetFade(Fader::STATE::FADE_OUT);
}

void SceneManager::SetPostEffect(const std::shared_ptr<PixelRenderer> _postEffectRenderer)
{
	//寿命チェック
	postEffectRenderers_.push_back(_postEffectRenderer);
}

void SceneManager::DeletePostEffect(const std::shared_ptr<PixelRenderer> _postEffectRenderer)
{
	//消したいrendererと同じものを探索する(owner_beforeで探索できる)
	auto it = std::find(
		postEffectRenderers_.begin()
		, postEffectRenderers_.end()
		, _postEffectRenderer);

	//特定のポストエフェクトを削除
	if (it != postEffectRenderers_.end())
	{
		postEffectRenderers_.erase(it);
	}
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	//ボタンUIはシーン通して使う
	ButtonUIManager::CreateInstance();

	scenes_.clear();
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = DELTA_TIME;

	camera_ = nullptr;

	totalTime_ = -1.0f;

	createScenePtr_ = {
		{SCENE_ID::TITLE,[this]() {return std::make_shared<TitleScene>(); }},
		{SCENE_ID::GAME,[this]() {return std::make_shared<GameScene>(); }},
		{SCENE_ID::REVOLUTION_START,[this]() {return std::make_shared<GameEventRevolutionScene>(); }},
		{SCENE_ID::GAME_CLEAR,[this]() {return std::make_shared<GameClearScene>(); }},
		{SCENE_ID::GAME_OVER,[this]() {return std::make_shared<GameOverScene>(); }}
	};
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = DELTA_TIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	// リソースの解放
	ResourceManager::GetInstance().SceneChangeRelease();

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放（空チェックあり）
	if (!scenes_.empty() && scenes_.back() != nullptr)
	{
		//シーンチェンジ時する前に解放(ゲーム終了時、Manager関連が解放されたのちにManagerの処理が通り、例外スローを避けるため)
		scenes_.back()->Release();	
		scenes_.back().reset();
		scenes_.pop_back(); // シーンを使い終わったのでリストからも削除
	}

	//シーン生成
	CreateScene(sceneId_);

	//デルタタイムリセット
	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;
}

const Fader& SceneManager::GetFader(void)
{
	return *fader_;
}

void SceneManager::Fade(void)
{
	Fader::STATE fState = fader_->GetState();
	isEndFade_ = false;
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isEndFade_ = true;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			isEndFade_ = true;
		}
		break;
	}
}

void SceneManager::SceneChangeFade(void)
{
	if (isEndFade_)
	{
		DoChangeScene(waitSceneId_);
		fader_->SetFade(Fader::STATE::NONE);
	}
}

void SceneManager::MainScreenDraw(void)
{
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw();

	// 描画
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}

	//カメラ
	camera_->Draw();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void SceneManager::PostEffectScreen(void)
{
	SetDrawScreen(postEffScreen_);
	// 画面を初期化
	ClearDrawScreen();

	// メインスクリーンを画面に描画する
	DrawGraph(0, 0, mainScreen_, false);

	//ポストエフェクトがあれば描画する
	for (const auto& postEff : postEffectRenderers_)
	{
		postEff->Draw();
	}

	// 暗転・明転
	fader_->Draw();

}

void SceneManager::MakeShadowMapTexture(void)
{
	// 描画可能テクスチャをシャドウマップ用にする
	SetCreateDrawValidGraphChannelNum(1);

	// 描画可能テクスチャを浮動小数点型で作成するフラグを有効にする
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);

	// 描画可能テクスチャの色深度を設定する
	SetCreateGraphColorBitDepth(COLOR_BIT_DEPTH);

	// シャドウマップ用テクスチャの作成
	shadowMapTexture_ = MakeScreen(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, false);

	// 描画可能テクスチャを通常の設定に戻す
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);

	// 描画可能テクスチャのチャンネル数を元に戻す
	SetCreateDrawValidGraphChannelNum(DEFAULT_CHANNEL_NUM);

	// 描画可能テクスチャの色深度を元に戻す
	SetCreateGraphChannelBitDepth(0);
}
