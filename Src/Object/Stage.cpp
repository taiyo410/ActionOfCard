#include "../Utility/Utility3D.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include"../Object/Common/Geometry/Model.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Load(void)
{
}

void Stage::Init(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	trans_.pos = Utility3D::VECTOR_ZERO;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	trans_.scl = { STAGE_SCL ,STAGE_SCL_Y ,STAGE_SCL };

	wallTrans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_WALL));
	wallTrans_.pos = Utility3D::VECTOR_ZERO;
	wallTrans_.quaRotLocal=
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	wallTrans_.scl= { WALL_SCL ,WALL_SCL ,WALL_SCL };

	scnMng_.GetCamera().lock()->SetStageTransform(&wallTrans_);

	//カプセル
	tag_ = Collider::TAG::STAGE;

	//当たり判定作成
	std::unique_ptr<Geometry>geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider(TAG_PRIORITY::BODY,{ tag_ }, std::move(geo), { Collider::TAG::NML_ATK });

	//マテリアル
	material_=std::make_unique<ModelMaterial>(
		ResourceManager::SRC::STAGE_VS,
		ResourceManager::SRC::STAGE_PS
	);
	material_->AddConstBufVS({ STAGE_UV_SCL,0.0f,0.0f,0.0f });
	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId,*material_);

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
	MV1DrawModel(wallTrans_.modelId);
	renderer_->Draw();

}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
