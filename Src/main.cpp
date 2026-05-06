#define _CRTDBG_MAP_ALLOC
#include <DxLib.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>
#include "Application.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

// WinMain関数
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// メモリリークを検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	// インスタンスの生成
	Application::CreateInstance();	
	
	// インスタンスの取得
	Application& instance = Application::GetInstance();

	// 初期化
	if (!instance.Init())
	{
		// 初期化失敗
		return -1;
	}

	// 実行
	instance.Run();

	// リソースの破棄
	if(!instance.Release())
	{
		// 初期化失敗
		return -1;
	}

	// インスタンスの破棄
	instance.Destroy();

	return 0;
}
