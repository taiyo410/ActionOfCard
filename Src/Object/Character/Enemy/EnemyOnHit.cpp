#include "../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Card/CardSystem.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../../../Utility/Utility3D.h"
#include "../Object/Character/Player/ActionController.h"
#include "../Object/Character/Player/Weapon.h"
#include "../Object/Character/Base/LogicBase.h"
#include "../Base/CharacterOnHitBase.h"
#include "EnemyOnHit.h"

EnemyOnHit::EnemyOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff
	, ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans):
	CharacterOnHitBase(_chara, _movedPos, _moveDiff, _action, _colParam, _trans)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;

	//衝突後処理の格納
	colUpdates_ = {
		{ TAG::PLAYER1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); } },
		{ TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); } }
	};
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::Init(void)
{
}

void EnemyOnHit::Load(void)
{
}

#ifdef _DEBUG
void EnemyOnHit::DrawDebug(void)
{
}
#endif // _DEBUG

void EnemyOnHit::CollNormalAttack(const std::weak_ptr<Collider> _hitCol)
{
	//親の情報を取得
	auto& parent = _hitCol.lock()->GetParent();

	//武器の取得
	auto& weapon = dynamic_cast<Weapon&>(parent);
	if (weapon.GetIsDamage()||charaObj_.GetCardAction()==CardActionBase::CARD_ACT_TYPE::JUMP_ATK)return;

	//ダメージを与えたことを知らせる
	weapon.SetIsDamage();

	//相手キャラの技攻撃力取得
	const float& atkPoint = action_.GetInput().GetTargetCharacter().lock()->GetMainAction().GetAtkPoint();

	//相手の数字との差が近いほどダメージを追加する
	const int cardDif= CardSystem::GetInstance().GetCardDif();

	int addDam = 0;
	//カードシステムからカードの強さの差を取得。
	cardDif == 0 ? addDam = 0 : addDam = MAX_ADD_DAMAGE - cardDif;

	//ダメージ計算
	int damage = static_cast<int>(atkPoint) + addDam;
	charaObj_.Damage(damage);

	action_.ChangeAction(ActionController::ACTION_TYPE::REACT);
}

void EnemyOnHit::CollChara(const std::weak_ptr<Collider> _hitCol)
{
	//相手と当たった
	isHitTarget_ = true;
}
