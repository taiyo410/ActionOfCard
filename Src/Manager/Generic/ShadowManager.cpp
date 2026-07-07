#include "pch.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/Camera.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Game/CharacterManager.h"
#include "ShadowManager.h"

ShadowManager::ShadowManager(void):
	scnMng_(SceneManager::GetInstance()),
	charaMng_(CharacterManager::GetInstance()),
	shadowPsHandle_(UtilityCommon::INITIAL_HANDLE),
	shadowMeshVSHandle_(UtilityCommon::INITIAL_HANDLE),
	shadowSkinnedMeshVSHandle_(UtilityCommon::INITIAL_HANDLE)
{
	lightViewMatrix_ = MATRIX{};
	lightProjectionMatrix_ = MATRIX{};
}

void ShadowManager::Init(void)
{	// シャドウマップハンドルの作成
	shadowMapTexture_ = MakeShadowMap(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

	//シャドウマップが想定するライト方向
	SetShadowMapLightDirection(shadowMapTexture_, SceneManager::LIGHT_DIR);

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(shadowMapTexture_,
		VGet(-SHADOWMAP_SIZE_HORIZON, -SHADOWMAP_SIZE_VERTICAL, -SHADOWMAP_SIZE_HORIZON),
		VGet(SHADOWMAP_SIZE_HORIZON, SHADOWMAP_SIZE_VERTICAL, SHADOWMAP_SIZE_HORIZON));
	////シャドウマップ生成用シェーダーハンドルの取得
	//ResourceManager& resMng = ResourceManager::GetInstance();
	//shadowPsHandle_ = resMng.Load(ResourceManager::SRC::SHADOW_PS).handleId_;
	//shadowMeshVSHandle_ = resMng.Load(ResourceManager::SRC::SHADOW_MESH_VS).handleId_;
	//shadowSkinnedMeshVSHandle_ = resMng.Load(ResourceManager::SRC::SHADOW_SKINNED_MESH_VS).handleId_;
}

void ShadowManager::DrawInitShadow(void)
{
	//影用深度記録画像をクリアする
	constexpr int MAX_COLOR = 255;
	SetBackgroundColor(MAX_COLOR, MAX_COLOR, MAX_COLOR);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);

	//カメラをシャドウマップ用に切り替える
	scnMng_.GetCamera().lock()->SetShadowCamera();
}

void ShadowManager::DrawStartSetUp(void)
{
	//描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
	//ライトの向き
	SetLightDirection(SceneManager::LIGHT_DIR);
	//シャドウマップ位置を計算(カメラ基準)
	VECTOR cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	//最も低い位置を出す
	VECTOR minPos = cameraPos;
	minPos.x -= SHADOWMAP_SIZE_HORIZON;
	minPos.y -= SHADOWMAP_SIZE_VERTICAL;
	minPos.z -= SHADOWMAP_SIZE_HORIZON;
	//最も高い位置
	VECTOR maxPos = cameraPos;
	maxPos.x += SHADOWMAP_SIZE_HORIZON;
	maxPos.y += SHADOWMAP_SIZE_VERTICAL;
	maxPos.z += SHADOWMAP_SIZE_HORIZON;

	SetShadowMapDrawArea(shadowMapTexture_, minPos, maxPos);


	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(shadowMapTexture_);
}

void ShadowManager::DrawEndSetUp(void)
{
	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, shadowMapTexture_);

	// ─── DXライブラリから自動生成された正確なシャドウマップ行列を取得 ───
	GetShadowMapViewProjectionMatrix(shadowMapTexture_, &shadowMat_);

}

void ShadowManager::DrawShadowNormal(void)
{
	// オリジナルシェーダー使用の設定
	MV1SetUseOrigShader(TRUE);

	// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
	SetUseVertexShader(shadowMeshVSHandle_);

	// 深度記録画像への描画用のピクセルシェーダーをセット
	SetUsePixelShader(shadowPsHandle_);
}

void ShadowManager::DrawShadowSkinned(void)
{
	//設定したカメラのビュー行列と射影行列を取得しておく
	lightViewMatrix_ = GetCameraViewportMatrix();
	lightProjectionMatrix_ = GetCameraProjectionMatrix();

	//影用深度記録画像をクリアする
	scnMng_.GetCamera().lock()->SetShadowCamera();
	//オリジナルシェーダー使用の再設定
	MV1SetUseOrigShader(TRUE);

	// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
	SetUseVertexShader(shadowSkinnedMeshVSHandle_);
}

void ShadowManager::ResetShader(void)
{
	// Shaderを元に戻す
	MV1SetUseOrigShader(FALSE);
	SetUseVertexShader(-1);
	SetUsePixelShader(-1);
	// 前回使用分のテクスチャを引き継がないように
	SetUseTextureToShader(0, -1);
}
