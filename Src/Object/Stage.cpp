#include "Utility/Utility3D.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/ShadowManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Generic/Camera.h"
#include "Renderer/ModelMaterial.h"
#include "Renderer/ModelRenderer.h"
#include "Object/Common/Geometry/Model.h"
#include "./Stage.h"

Stage::Stage(void)
{
	objectName_ = STAGE_STR;
}

Stage::~Stage(void)
{
}

void Stage::Load(void)
{

}

void Stage::Init(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_FLOOR));
	trans_.pos = Utility3D::VECTOR_ZERO;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	trans_.scl = { STAGE_SCL ,STAGE_SCL_Y ,STAGE_SCL };

	wallTrans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_WALL));
	wallTrans_.pos = Utility3D::VECTOR_ZERO;
	wallTrans_.quaRotLocal=
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	wallTrans_.scl= { WALL_SCL ,WALL_SCL ,WALL_SCL };

	scnMng_.GetCamera().lock()->SetStageTransform(&trans_);

	//カプセル
	tag_ = Collider::TAG::STAGE;

	//当たり判定作成
	MakeColliderFromJsonData();

	SettingShader();

	//モデル更新
	trans_.Update();

	//壁
	wallTrans_.Update();
}

void Stage::Update(void)
{
	trans_.Update();
}

void Stage::Draw(void)
{
	MATRIX lightVP =shadowMng_.GetShadowMapViewProjectionMat();
	const int shadowMapTex = shadowMng_.GetShadowMapTexture();

	material_->SetTextureBuf(SHADOW_MAP_TEXTURE_NUM, shadowMapTex);
	material_->SetConstBufVSMatrix(0, lightVP);
	wallMaterial_->SetTextureBuf(SHADOW_MAP_TEXTURE_NUM, shadowMapTex);
	wallMaterial_->SetConstBufVSMatrix(0,lightVP);

	renderer_->Draw();
	wallRenderer_->Draw();
}

void Stage::DrawShadow(void)
{
	MV1DrawModel(trans_.modelId);
	MV1DrawModel(wallTrans_.modelId);
}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void Stage::SettingShader(void)
{
	//光の方向を取得
	VECTOR worldLightDir = SceneManager::LIGHT_DIR;
	const int shadowMapHandle = shadowMng_.GetShadowMapTexture();
	MATRIX lightVP = shadowMng_.GetShadowMapViewProjectionMat();

	//マテリアル
	material_ = std::make_unique<ModelMaterial>(
		ResourceManager::SRC::STANDARD_VS,
		ResourceManager::SRC::STANDARD_PS
	);
	material_->SetTextureBuf(SHADOW_MAP_TEXTURE_NUM, shadowMapHandle);
	material_->AddConstBufPS(UtilityCommon::DEFAULT_FLOAT4);
	material_->AddConstBufPS({ worldLightDir.x,worldLightDir.y,worldLightDir.z,1.0f });
	material_->AddConstBufPS(UtilityCommon::FLOAT4_ZERO);
	material_->AddConstBufVS({ STAGE_UV_SCL,0.0f,0.0f,0.0f });
	material_->AddConstBufVSMatrix(lightVP);
	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId, *material_);

	//壁のマテリアルとレンダラー
	wallMaterial_ = std::make_unique<ModelMaterial>(
		ResourceManager::SRC::STANDARD_VS,
		ResourceManager::SRC::STANDARD_PS
	);
	wallMaterial_->SetTextureBuf(SHADOW_MAP_TEXTURE_NUM, shadowMapHandle);
	wallMaterial_->AddConstBufPS(UtilityCommon::DEFAULT_FLOAT4);
	wallMaterial_->AddConstBufPS({ worldLightDir.x,worldLightDir.y,worldLightDir.z,1.0f });
	wallMaterial_->AddConstBufPS(UtilityCommon::FLOAT4_ZERO);
	wallMaterial_->AddConstBufVS(UtilityCommon::DEFAULT_FLOAT4);
	wallMaterial_->AddConstBufVSMatrix(lightVP);
	wallRenderer_ = std::make_unique<ModelRenderer>(wallTrans_.modelId, *wallMaterial_);
}
