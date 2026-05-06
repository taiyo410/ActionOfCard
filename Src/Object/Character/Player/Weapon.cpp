#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/ModelFrameUtility.h"
#include "../Object/Common/EffectController.h"
#include "../Object/Common/Geometry/Geometry.h"
#include "../Object/Common/Geometry/Capsule.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "Weapon.h"

Weapon::Weapon(CharacterBase& _chara):
character_(_chara),
targetTrans_(nullptr),
followFrameNo_(0),
localPos_(Utility3D::VECTOR_ZERO),
localRot_(Utility3D::VECTOR_ZERO),
isDamage_(false)
{
	effect_ = std::make_unique<EffectController>();
}

Weapon::~Weapon(void)
{
}

void Weapon::Load(void)
{
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::KEY_BLADE));
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::KEY_BLADE_HIT_EFF).handleId_,
		EffectController::EFF_TYPE::KEY_BLADE_HIT);

	//リソースロード
	resMng_.Load(ResourceManager::SRC::PLAYER_HIT_SE);
	resMng_.Load(ResourceManager::SRC::PLAYER_ATTACK_SE);
}

void Weapon::Init(void)
{
	//武器の相対位置初期化
	localPos_ = { 0.0f,0.0f,0.0f };

	//武器の相対回転初期化
	localRot_ = { 0.0f,UtilityCommon::Deg2RadF(LOCAL_ANGLE_DEG.y),UtilityCommon::Deg2RadF(LOCAL_ANGLE_DEG.z) };

	//タグ付け
	tag_ = Collider::TAG::PLAYER1;

	//モデル情報初期化
	trans_.scl = { WEAPON_SCL,WEAPON_SCL, WEAPON_SCL };
}

void Weapon::Update(void)
{
	// 対象フレームの位置にtargetを配置し、
	// 対象フレームの回転に加え、指定した相対座標・回転を加える
	ModelFrameUtility::SetFrameWorldMatrix(
		*targetTrans_, followFrameNo_, trans_,
		localPos_, localRot_
	);

	//モデル情報から角度を取得し、常に同期
	trans_.quaRot = Quaternion::GetRotation(trans_.matRot);
	trans_.Update();
}

void Weapon::Draw(void)
{
	MV1DrawModel(trans_.modelId);
}

void Weapon::SetTargetAndFrameNo(Transform* _targetTrans, int _frameNo)
{
	targetTrans_ = _targetTrans;
	followFrameNo_ = _frameNo;
}

void Weapon::MakeWeaponCollider(void)
{
	//ダメージフラグ初期化
	isDamage_ = false;

	//当たり判定をしないタグ設定
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::JUMP_ATK);

	//カプセル形状作成
	const VECTOR CAP_TOP = { 0.0f,CAPSULE_COL_HEIGHT ,0.0f };
	std::unique_ptr<Capsule> geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, CAP_TOP, CAPSULE_COL_RADIUS);
	MakeCollider(TAG_PRIORITY::ATK_SPHERE, { Collider::TAG::PLAYER1,Collider::TAG::NML_ATK }, std::move(geo), noneHitTag_);
	tagPrioritys_.emplace_back(TAG_PRIORITY::ATK_SPHERE);

	//攻撃発生時にSE再生
	SoundManager::GetInstance().Play(ResourceManager::SRC::PLAYER_ATTACK_SE, SoundManager::PLAYTYPE::BACK);
}

void Weapon::DeleteWeaponCollider(void)
{
	DeleteCollider(TAG_PRIORITY::ATK_SPHERE);
}

void Weapon::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	if (character_.GetIsDamage())return;
	//エフェクト再生
	character_.SetIsDamage();

	//ヒットストップで一瞬だけ処理を止める
	character_.ChangeUpdatePhase(CharacterBase::UPDATE_PHASE::HIT_STOP);

	//カメラシェイク
	scnMng_.GetInstance().GetCamera().lock()->SetShakeStatus(-1.0f, 100.0f, Easing::EASING_TYPE::ELASTIC_BACK, 0.5f);
	scnMng_.GetInstance().GetCamera().lock()->ChangeSub(Camera::SUB_MODE::ONE_SHAKE);

	//ヒットエフェクトを再生
	VECTOR bladeFramePos = MV1GetFramePosition(trans_.modelId, EFFECT_PLAY_FRAME_NO);
	effect_->Play(EffectController::EFF_TYPE::KEY_BLADE_HIT, bladeFramePos, {}, { EFFECT_PLAY_SCL,EFFECT_PLAY_SCL,EFFECT_PLAY_SCL });
	SoundManager::GetInstance().Play(ResourceManager::SRC::PLAYER_HIT_SE, SoundManager::PLAYTYPE::BACK);
}
