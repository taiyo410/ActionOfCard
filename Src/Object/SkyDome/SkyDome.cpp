#include "../../Common/Quaternion.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "../Common/Easing.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "SkyDome.h"

SkyDome::SkyDome(void):
	followState_(FOLLOW_STATE::NONE),
	phase_(PHASE::NONE),
	colEasingCnt_(0.0f)
{
	phaseChanges_ = {
		{PHASE::NONE,updatePhase_ = [this]() {UpdateNone(); }},
		{PHASE::BATTLE,updatePhase_ = [this]() {UpdateNone(); }},
		{PHASE::CLEAR,updatePhase_ = [this]() {UpdateClear(); }}
	};

	easing_ = std::make_unique<Easing>();
}

void SkyDome::Load(void)
{
	//リソース読み込み
	trans_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
}

void SkyDome::Init(void)
{
	// モデル制御の基本情報
	trans_.scl = SCALES;
	trans_.quaRot = Quaternion::Euler(
		0.0f,
		UtilityCommon::Deg2RadF(LOCAL_DEG),
		0.0f);
	trans_.quaRotLocal = Quaternion();

	//座標位置をずらす
	trans_.pos = {
		0.0f, 
		0.0f, 
		0.0f };
	trans_.Update();

	material_ = std::make_unique<ModelMaterial>(
		ResourceManager::SRC::STAGE_VS,
		ResourceManager::SRC::SKYDOME_PS
	);
	material_->AddConstBufVS({ SIZE_SCL,0.0f,0.0f,0.0f });
	material_->AddConstBufPS({ COLOR_SCL_BATTLE_R, COLOR_SCL_BATTLE_G, COLOR_SCL_BATTLE_B, 1.0f });
	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId, *material_);

	ChangePhase(PHASE::BATTLE);

	//Zバッファを無効
	MV1SetUseZBuffer(trans_.modelId, false);
	MV1SetWriteZBuffer(trans_.modelId, false);
}

void SkyDome::Update(void)
{
	updatePhase_();
	Quaternion yRot = Quaternion::AngleAxis(UtilityCommon::Deg2RadF(ROT_SPEED), Utility3D::AXIS_Y);
	trans_.quaRot = Quaternion::Mult(trans_.quaRot, yRot);
	trans_.Update();
}

void SkyDome::Draw(void)
{
	//MV1DrawModel(transform_.modelId);
	renderer_->Draw();
}

void SkyDome::ChangePhase(const PHASE _phase)
{
	if (phase_ == _phase)return;
	phase_ = _phase;
	phaseChanges_[phase_]();
}

void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateClear(void)
{
	if (colEasingCnt_ > COLOR_EASE_TIME)
	{
		return;
	}
	VECTOR col = easing_->EaseFunc(COLOR_SCL_BATTLE
	, COLOR_SCL_DEFAULT, colEasingCnt_ / COLOR_EASE_TIME,Easing::EASING_TYPE::LERP);
	colEasingCnt_ += scnMng_.GetDeltaTime();
	material_->SetConstBufPS(0, { col.x,col.y,col.z,1.0f });
}
