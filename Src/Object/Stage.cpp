#include "Utility/Utility3D.h"
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/SceneManager.h"
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

	//光の方向を取得
	VECTOR worldRightDir = GetLightDirection();
	//マテリアル
	material_=std::make_unique<ModelMaterial>(
		ResourceManager::SRC::TILEMAP_VS,
		ResourceManager::SRC::SHADOW_STANDARD_PS
	);
	material_->SetTextureBuf(8, scnMng_.GetShadowMapTexture());
	material_->AddConstBufVS({ STAGE_UV_SCL,0.0f,0.0f,0.0f });
	material_->AddConstBufPS({ worldRightDir.x,worldRightDir.y,worldRightDir.z,1.0f });
	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId,*material_);

	//壁のマテリアルとレンダラー
	wallMaterial_ = std::make_unique<ModelMaterial>(
		ResourceManager::SRC::STANDARD_VS,
		ResourceManager::SRC::SHADOW_STANDARD_PS
	);

	//壁の色の乗算入らないので1.0を設定
	wallMaterial_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	wallMaterial_->SetTextureBuf(8, scnMng_.GetShadowMapTexture());
	wallMaterial_->AddConstBufPS({ worldRightDir.x,worldRightDir.y,worldRightDir.z,1.0f });
	//環境光を追加
	wallMaterial_->AddConstBufPS({ AMBIENT_VEC.x,AMBIENT_VEC.y,AMBIENT_VEC.z,0.0f });
	wallRenderer_ = std::make_unique<ModelRenderer>(wallTrans_.modelId,*wallMaterial_);

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
	wallRenderer_->Draw();
	renderer_->Draw();
}

void Stage::DrawShadow(void)
{
}

void DrawShadow(void)
{
}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
