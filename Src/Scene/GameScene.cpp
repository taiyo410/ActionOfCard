#include <DxLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Character/Enemy/Enemy.h"
#include "../Object/Card/CardSystem.h"	
#include "../Object/Stage.h"	
#include "PauseScene.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updateFunc_ = [this]() {LoadingUpdate(); };

	//描画関数のセット
	drawFunc_ = [this]() {LoadingDraw(); };

	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	changeUpdate_ = {
	{UPDATE_PHASE::NONE,[this]() {ChangeNone(); }},
	{UPDATE_PHASE::FADE,[this]() {ChangeFade(); }},
	{UPDATE_PHASE::DIRECTION,[this]() {ChangeDirection(); }},
	{UPDATE_PHASE::CLEAR_DIRECTION,[this]() {ChangeClearDirection(); }},
	{UPDATE_PHASE::OVER_DIRECTION,[this]() {ChangeOverDirection(); }},
	{UPDATE_PHASE::NORMAL,[this]() {ChangeNormal(); }},
	{UPDATE_PHASE::SLOW,[this]() {ChangeSlow(); }}
	};

	CardSystem::CreateInstance();
	UIManager::CreateInstance();
	CharacterManager::CreateInstance();
	CollisionManager::CreateInstance();
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	CardSystem::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	UIManager::GetInstance().Destroy();
}

void GameScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	//ポーズ画面のリソース
	pauseScene_ = std::make_shared<PauseScene>();
	pauseScene_->Load();

	//UIマネージャ
	UIManager::GetInstance().Load();

	//ボタンUIマネージャ
	ButtonUIManager::GetInstance().Load();

	//ステージ
	stage_ = std::make_unique<Stage>();

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Load();

	//キャラクター
	CharacterManager::GetInstance().Load();
}

void GameScene::Init(void)
{
	updatePhase_ = UPDATE_PHASE::NONE;

	//演出へ
	ChangeUpdatePhase(UPDATE_PHASE::DIRECTION);

	CharacterManager::GetInstance().Init();
	UIManager::GetInstance().Init();

	//シェイク状態を初期化
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	stage_->Init();
	skyDome_->Init();
	resMng_.Load(ResourceManager::SRC::GAME_BGM);
	soundMng_.Play(ResourceManager::SRC::GAME_BGM, SoundManager::PLAYTYPE::LOOP);
}

void GameScene::Release(void)
{
	soundMng_.AllStop();
}

void GameScene::CheckSkip(void)
{
	//Yボタン長押しでスキップする
	if (inputMng_.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHTBUTTON_TOP))
	{
		if (skipKeepCnt_ > SKIP_BTN_TIME)
		{
			Skip();
		}
		skipKeepCnt_ += scnMng_.GetDeltaTime();
	}
	else
	{
		skipKeepCnt_ = 0.0f;
	}

	//割合をスキップゲージにセットする
	float per = skipKeepCnt_ / SKIP_BTN_TIME;
	UIManager::GetInstance().SetSkipPer(per);

	//スキップされたら演出を終了し、フェードイン
	if (isSkippingDirection_)
	{
		scnMng_.Fade();
		if (scnMng_.GetIsEndFade())
		{
			ChangeUpdatePhase(UPDATE_PHASE::FADE);
		}
	}
}

void GameScene::NoneUpdate(void)
{
	//何もしない
}

void GameScene::FadeUpdate(void)
{
	scnMng_.Fade();

	//フェード終了後、通常処理へ
	if (scnMng_.GetIsEndFade())
	{
		ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
	}
}

void GameScene::NormalUpdate(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_ESCAPE)||inputMng_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::SELECT_BUTTON))
	{
		scnMng_.PushScene(pauseScene_);
		return;
	}
	//敵が倒れたらクリアシ－ンへ
	if (CharacterManager::GetInstance().IsSceneChageClearCondition())
	{
		ChangeUpdatePhase(UPDATE_PHASE::CLEAR_DIRECTION);
		return;
	}
	//自分が倒れたらゲームオーバーシーンへ
	else if (CharacterManager::GetInstance().IsSceneChangeGameOverCondition())
	{
		ChangeUpdatePhase(UPDATE_PHASE::OVER_DIRECTION);
		return;
	}

	//ステージ
	stage_->Update();

	//キャラクターの更新
	CharacterManager::GetInstance().Update();

	//カード勝敗状態の監視
	CardSystem::GetInstance().CompareCards();

	//UIの更新
	UIManager::GetInstance().Update();

	//更新はアクション中のみ
	CollisionManager::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();
}

void GameScene::NormalDraw(void)
{
	//プレイヤー
	skyDome_->Draw();

	//ステージ
	stage_->Draw();

	//キャラクター
	CharacterManager::GetInstance().Draw();
	
	//UI
	UIManager::GetInstance().Draw();
}

void GameScene::DirectionDraw(void)
{
	//プレイヤー
	skyDome_->Draw();

	//ステージ
	stage_->Draw();

	//キャラクター
	CharacterManager::GetInstance().Draw();

	//演出時の2D描画
	if (updatePhase_ == UPDATE_PHASE::DIRECTION)
	{
		UIManager::GetInstance().DirectionDraw();
	}

}


void GameScene::ChangeUpdatePhase(const UPDATE_PHASE _phase)
{
	if (updatePhase_ == _phase)return;
	updatePhase_ = _phase;
	changeUpdate_[updatePhase_]();
}

void GameScene::ChangeNone(void)
{
}

void GameScene::ChangeFade(void)
{
	//フェード開始
	scnMng_.StartFadeIn();

	//スキップ
	isSkippingDirection_ = true;

	//カメラ遷移
	scnMng_.GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

	updateFunc_ = [this]() {FadeUpdate(); };
}

void GameScene::ChangeDirection(void)
{
	//スキップフラグ初期化
	isSkippingDirection_ = false;

	//キャラクターの演出更新
	CharacterManager::GetInstance().ChangeCharacterDirectionUpdate();

	updateFunc_ = [this]() {DirectionUpdate(); };
	drawFunc_ = [this]() {DirectionDraw(); };
}

void GameScene::ChangeClearDirection()
{
	//キャラクターをクリア状態へ
	CharacterManager::GetInstance().ChangeCharacterClearDirection();

	//カメラのリセット
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	//スカイドームをクリア状態へ
	skyDome_->ChangePhase(SkyDome::PHASE::CLEAR);

	updateFunc_ = [this]() {ClearDirectionUpdate(); };
	drawFunc_ = [this]() {DirectionDraw(); };
}

void GameScene::ChangeOverDirection(void)
{
	//キャラクターをゲームオーバーへ
	CharacterManager::GetInstance().ChangeCharacterOverDirection();

	//カメラのリセット
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	updateFunc_ = [this]() {OverDirectionUpdate(); };
	drawFunc_ = [this]() {DirectionDraw(); };
}

void GameScene::ChangeNormal(void)
{
	//キャラクターを通常更新へ
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();

	//カメラのリセット
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}

void GameScene::ChangeSlow(void)
{
	//スロー状態へ
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();
	updateFunc_ = [this]() {SlowUpdate(); };
}

void GameScene::DirectionUpdate(void)
{
	//スキップ
	CheckSkip();

	//演出UI
	UIManager::GetInstance().DirectionUpdate();

	//フェードアウトしているとき、更新を飛ばす
	if (scnMng_.GetFader().GetState()==Fader::STATE::FADE_OUT)return;

	//演出終了後、通常カメラへ
	if (scnMng_.GetCamera().lock()->IsEndDirectionMode())
	{
		ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
		return;
	}

	//キャラクターの更新
	CharacterManager::GetInstance().Update();
}

void GameScene::ClearDirectionUpdate(void)
{
	//敵の演出が終わったらシーン遷移
	if (CharacterManager::GetInstance().GetIsEndClearDirection())
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
	}

	//キャラクター
	CharacterManager::GetInstance().Update();

	//スカイドーム
	skyDome_->Update();
}

void GameScene::OverDirectionUpdate(void)
{
	//敵の演出が終わったらシーン遷移
	if (CharacterManager::GetInstance().GetIsEndOverDirection())
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME_OVER);
	}

	//キャラクター
	CharacterManager::GetInstance().Update();
}

void GameScene::SlowUpdate(void)
{
	//2フレームに1回更新
	if (--slowFrame_ > 0)return;
	slowFrame_ = FRAME_PER_UPDATE;

	//通常更新
	NormalUpdate();
}

void GameScene::OnSceneEnter(void)
{
	//演出状態へ移行
	ChangeDirection();
}
void GameScene::Skip(void)
{
	//すでにtrueの場合、処理を飛ばす
	if (isSkippingDirection_)return;

	//フェード開始
	scnMng_.StartFadeOut();

	//演出スキップフラグ
	isSkippingDirection_ = true;
}