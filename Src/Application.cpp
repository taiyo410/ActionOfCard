#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/InputManagerS.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Resource/FontManager.h"
#include "Manager/Resource/SoundManager.h"
#include "FpsControl/FpsControl.h"
#include "Application.h"

const std::wstring Application::PATH_IMAGE = L"Data/Image/";
const std::wstring Application::PATH_MODEL = L"Data/Model/";
const std::wstring Application::PATH_ANIM_PLAYER = L"Data/Model/Animation/Player/";
const std::wstring Application::PATH_ANIM_ENEMY = L"Data/Model/Animation/Enemy/";
const std::wstring Application::PATH_EFFECT = L"Data/Effect/";
const std::wstring Application::PATH_SOUND = L"Data/Sound/";
const std::wstring Application::PATH_SOUND_BGM = L"Data/Sound/BGM/";
const std::wstring Application::PATH_SOUND_SE = L"Data/Sound/SE/";
const std::wstring Application::PATH_FONT = L"Data/Font/";
const std::wstring Application::PATH_TEXT = L"Data/Text/";
const std::wstring Application::PATH_JSON = L"Data/Json/";
const std::wstring Application::PATH_CSV = L"Data/CSV/";
const std::wstring Application::PATH_SHADER = L"Data/Shader/";

bool Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText(WINDOW_NAME.c_str());

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_DEPTH);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		return false;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();			//生成
	InputManagerS::CreateInstance();			//生成
	InputManager::GetInstance().Init();		//初期化
	InputManagerS::GetInstance().Init();		//初期化

	// リソース管理初期化
	ResourceManager::CreateInstance();
	ResourceManager::GetInstance().Init();

	//サウンド管理初期化
	SoundManager::CreateInstance();
	SoundManager::GetInstance().Init();

	// シーン管理初期化
	SceneManager::CreateInstance();		
	SceneManager::GetInstance().Init();	


	// FPS初期化
	fps_ = std::make_unique<FpsControl>();
	fps_->Init();

	// フォント管理初期化
	fontMng_ = std::make_unique<FontManager>();
	fontMng_->Init();

	isGameEnd_ = false;

	return true;
}

void Application::Run(void)
{
	LONGLONG time = GetNowHiPerformanceCount();

	auto& inputManager = InputManager::GetInstance();
	auto& inputManagerS = InputManagerS::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 &&!isGameEnd_)
	{
		//フレームレートを更新
		if (!fps_->UpdateFrameRate()) continue;

		//更新処理
		inputManager.Update();
		inputManagerS.Update();
		sceneManager.Update();

		//描画処理
		sceneManager.Draw();

		//フレームレート計算
		fps_->CalcFrameRate();

#ifdef _DEBUG
		fps_->DrawFrameRate();
#endif // _DEBUG

		ScreenFlip();
	}

}

bool Application::Release(void)
{
	//各クラスのリソースの破棄
	InputManager::GetInstance().Release();
	InputManagerS::GetInstance().Release();
	SceneManager::GetInstance().Release();
	ResourceManager::GetInstance().Release();

	//インスタンスの破棄
	fontMng_->Destroy();
	InputManager::GetInstance().Destroy();
	InputManagerS::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		return false;
	}

	return true;
}

Application::Application(void):
	isGameEnd_(false),
	fps_(nullptr),
	fontMng_(nullptr)
{

}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
