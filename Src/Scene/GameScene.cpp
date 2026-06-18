#include <DxLib.h>
#include "Utility/UtilityCommon.h"
#include "Application.h"
#include "Common/Fader.h"
#include "Manager/Resource/FontManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/ButtonUIManager.h"
#include "Manager/Generic/UIManager.h"
#include "Manager/Game/CollisionManager.h"
#include "Manager/Game/CharacterManager.h"
#include "Manager/Resource/SoundManager.h"
#include "Manager/Generic/DataBank.h"
#include "Renderer/PixelMaterial.h"
#include "Renderer/PixelRenderer.h"
#include "Object/SkyDome/SkyDome.h"
#include "Object/Card/CardSystem.h"	
#include "Object/Stage.h"	
#include "./PauseScene.h"
#include "./GameEventRevolutionScene.h"
#include "./GameScene.h"

GameScene::GameScene(void):
	revolutionCnt_(),
	fadeCnt_(),
	isSkippingDirection_(),
	revolutionRondomTime_(),
	skipKeepCnt_(),
	slowFrame_(),
	waitCnt_()
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

	pauseScene_ = std::make_shared<PauseScene>();
	revolutionScene_ = std::make_shared<GameEventRevolutionScene>();
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	CardSystem::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	UIManager::GetInstance().Destroy(); 
	changeUpdate_.clear();
}

void GameScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	LoadInvertEffect();			//反転フェードシェーダのロード
	ObjectLoad();				//オブジェクトのロード
}

void GameScene::Init(void)
{
	updatePhase_ = UPDATE_PHASE::NONE;

	//演出へ
	ChangeUpdatePhase(UPDATE_PHASE::DIRECTION);

	//革命になるランダム時間を決める
	float rand = UtilityCommon::GetMersenneRandomNumber(REVOLUTION_TIME_MIN, REVOLUTION_TIME_MAX);
	revolutionRondomTime_ = rand;
	revolutionCnt_ = revolutionRondomTime_;
	revolutionFadeFunc_ = [this]() {RevolutionInvertFadeNone(); };
	fadeCnt_ = 0.0f;
	waitCnt_ = WAIT_TIME;

	//シェイク状態を初期化
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
	resMng_.Load(ResourceManager::SRC::GAME_BGM);
	soundMng_.Play(ResourceManager::SRC::GAME_BGM, SoundManager::PLAYTYPE::LOOP);

	ObjectInit();
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
	//シーン変更されたら処理を飛ばす
	if (CheckGameStateTransition())return;

	//革命時間の更新
	RevolutionUpdate();

	//各オブジェクトの更新
	ObjectUpdate();

	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();
}

void GameScene::NormalDraw(void)
{
	ObjectDraw();
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

	//カメラモード変更
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::START_DIRECTION);

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

	//反転フェーダの削除
	scnMng_.DeletePostEffect(invertRenderer_);

	updateFunc_ = [this]() {ClearDirectionUpdate(); };
	drawFunc_ = [this]() {DirectionDraw(); };
}

void GameScene::ChangeOverDirection(void)
{
	//キャラクターをゲームオーバーへ
	CharacterManager::GetInstance().ChangeCharacterOverDirection();

	//カメラのリセット
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	//反転フェーダの削除
	scnMng_.DeletePostEffect(invertRenderer_);

	updateFunc_ = [this]() {OverDirectionUpdate(); };
	drawFunc_ = [this]() {DirectionDraw(); };
}

void GameScene::ChangeNormal(void)
{
	//キャラクターを通常更新へ
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();

	//カメラのリセット
	scnMng_.GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

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
	ChangeUpdatePhase(UPDATE_PHASE::DIRECTION);
}

void GameScene::ObjectLoad(void)
{
	pauseScene_->Load();					//ポーズ画面のリソース
	revolutionScene_->Load();				//革命シーン
	UIManager::GetInstance().Load();		//UIマネージャ
	ButtonUIManager::GetInstance().Load();	//ボタンUIマネージャ
	stage_ = std::make_unique<Stage>();		//ステージ
	skyDome_ = std::make_unique<SkyDome>();	//スカイドーム
	skyDome_->Load();
	CharacterManager::GetInstance().Load();	//キャラクター
}

void GameScene::ObjectInit(void)
{
	CharacterManager::GetInstance().Init();
	UIManager::GetInstance().Init();
	stage_->Init();
	skyDome_->Init();
}

void GameScene::ObjectUpdate(void)
{
	stage_->Update();	//ステージ
	CharacterManager::GetInstance().Update();	//キャラクターの更新
	CardSystem::GetInstance().CompareCards();	//カード勝敗状態の監視
	UIManager::GetInstance().Update();			//UIの更新
	CollisionManager::GetInstance().Update();	//更新はアクション中のみ
}

void GameScene::ObjectDraw(void)
{
	skyDome_->Draw();	//プレイヤー
	stage_->Draw();		//ステージ
	//キャラクター
	CharacterManager::GetInstance().Draw();
	CharacterManager::GetInstance().Draw2D();
	//UI
	UIManager::GetInstance().Draw();
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

void GameScene::LoadInvertEffect(void)
{
	//ポストエフェクトのシーンハンドル生成
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	invertMaterial_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::REVOLUTION_POSTEFF_PS);

	//メインスクリーン
	const int mainScreen = scnMng_.GetMainScreen();
	invertMaterial_->AddTextureBuf(mainScreen);

	//マスク画像
	const int maskImg = resMng_.Load(ResourceManager::SRC::REVERSE_FADE_MASK).handleId_;
	invertMaterial_->AddTextureBuf(maskImg);
	invertMaterial_->AddConstBuf({ fadeCnt_ ,0.0f,0.0f,0.0f });

	invertRenderer_ = std::make_shared<PixelRenderer>(*invertMaterial_);
	invertRenderer_->MakeScreenVertex();
}

void GameScene::RevolutionUpdate(void)
{
	//革命更新
	revolutionFadeFunc_();

	//シェーダー関連の更新
	const int mainScreen = scnMng_.GetMainScreen();
	invertMaterial_->SetTextureBuf(0, mainScreen);
	invertMaterial_->SetConstBuf(0, { fadeCnt_ / FADE_TIME ,0.0f,0.0f,0.0f });
}

bool GameScene::CheckGameStateTransition(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_ESCAPE) || inputMng_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::SELECT_BUTTON))
	{
		scnMng_.PushScene(pauseScene_);
		return true;
	}
	//敵が倒れたらクリアシ－ンへ
	if (CharacterManager::GetInstance().IsSceneChangeClearCondition())
	{
		ChangeUpdatePhase(UPDATE_PHASE::CLEAR_DIRECTION);
		return true;
	}
	//自分が倒れたらゲームオーバーシーンへ
	else if (CharacterManager::GetInstance().IsSceneChangeGameOverCondition())
	{
		ChangeUpdatePhase(UPDATE_PHASE::OVER_DIRECTION);
		return true;
	}
	return false;
}

void GameScene::RevolutionInvertFadeNone(void)
{
	//ランダム時間ごとに革命と中将ルールを切り替える
	if (revolutionCnt_ < 0.0f)
	{
		//ランダムで革命状態の時間を決める
		float rand = UtilityCommon::GetMersenneRandomNumber(REVOLUTION_TIME_MIN, REVOLUTION_TIME_MAX);
		revolutionRondomTime_ = rand;
		revolutionCnt_ = revolutionRondomTime_;

		//ルールを切り替える
		CardSystem::GetInstance().ChangeJudgeRule();
		scnMng_.SetPostEffect(invertRenderer_);
		revolutionFadeFunc_ = [this]() {RevolutionInvertFadeIn(); };
		return;
	}
	revolutionCnt_ -= scnMng_.GetDeltaTime();
}

void GameScene::RevolutionInvertFadeIn(void)
{
	if (fadeCnt_ < FADE_TIME)
	{
		fadeCnt_ += scnMng_.GetDeltaTime();
		return;
	}

	//フェード終了後、一定時間待機
	if (waitCnt_ < 0.0f)
	{
		waitCnt_ = WAIT_TIME;
		revolutionFadeFunc_ = [this]() {RevolutionInvertFadeOut(); };
		return;
	}
	waitCnt_ -= scnMng_.GetDeltaTime();
}

void GameScene::RevolutionInvertFadeOut(void)
{
	if (fadeCnt_ > 0.0f)
	{
		fadeCnt_ -= scnMng_.GetDeltaTime();
	}
	else
	{
		scnMng_.DeletePostEffect(invertRenderer_);
		fadeCnt_ = 0.0f;
		revolutionFadeFunc_ = [this]() {RevolutionInvertFadeNone(); };
	}
}