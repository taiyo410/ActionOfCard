#include "pch.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Game/CharacterManager.h"
#include "Shadow.h"

Shadow::Shadow(void):
	scnMng_(SceneManager::GetInstance()),
	charaMng_(CharacterManager::GetInstance()),
	shadowMapTexture_(UtilityCommon::INITIAL_HANDLE),
	shadowPsHandle_(UtilityCommon::INITIAL_HANDLE),
	shadowMeshVSHandle_(UtilityCommon::INITIAL_HANDLE),
	shadowSkinnedMeshVSHandle_(UtilityCommon::INITIAL_HANDLE)
{
}

Shadow::~Shadow(void)
{
}

void Shadow::Init(void)
{
	//シャドウマップ生成用シェーダーハンドルの取得
	ResourceManager& resMng = ResourceManager::GetInstance();
	shadowPsHandle_ = resMng.Load(ResourceManager::SRC::SHADOW_PS).handleId_;
	shadowMeshVSHandle_ = resMng.Load(ResourceManager::SRC::SHADOW_MESH_VS).handleId_;
	shadowSkinnedMeshVSHandle_ = resMng.Load(ResourceManager::SRC::SHADOW_SKINNED_MESH_VS).handleId_;
}

void Shadow::DrawInitShadow(void)
{
	//影用深度記録画像をクリアする
	constexpr int MAX_COLOR = 255;
	SetBackgroundColor(MAX_COLOR, MAX_COLOR, MAX_COLOR);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);

	//カメラをシャドウマップ用に切り替える
	scnMng_.GetCamera().lock()->ChangeMode(Camera::MODE::SHADOW_CAMERA);
}

void Shadow::DrawShadowNormal(void)
{
	scnMng_.GetCamera().lock()->ChangeMode(Camera::MODE::SHADOW_CAMERA);
	// オリジナルシェーダー使用の設定
	MV1SetUseOrigShader(TRUE);

	// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
	SetUseVertexShader(shadowMeshVSHandle_);

	// 深度記録画像への描画用のピクセルシェーダーをセット
	SetUsePixelShader(shadowPsHandle_);
}

void Shadow::DrawShadowSkinned(void)
{
	//影用深度記録画像をクリアする
	scnMng_.GetCamera().lock()->ChangeMode(Camera::MODE::SHADOW_CAMERA);
	SetDrawScreen(shadowMapTexture_);
	// オリジナルシェーダー使用の再設定
	MV1SetUseOrigShader(TRUE);

	// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
	SetUseVertexShader(shadowSkinnedMeshVSHandle_);
}

void Shadow::ResetShader(void)
{
	// Shaderを元に戻す
	MV1SetUseOrigShader(FALSE);
	SetUseVertexShader(-1);
	SetUsePixelShader(-1);
	// 前回使用分のテクスチャを引き継がないように
	SetUseTextureToShader(0, -1);
}
