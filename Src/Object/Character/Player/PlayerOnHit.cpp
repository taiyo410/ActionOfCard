#include <ranges>
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Enemy/EnemyRock.h"
#include"../../../Utility/Utility3D.h"
#include"./ActionController.h"
#include"./Player.h"
#include "../Base/CharacterOnHitBase.h"
#include "PlayerOnHit.h"
namespace
{
	using TAG_PRIORITY = ObjectBase::TAG_PRIORITY;
	using TAG = Collider::TAG;
}
PlayerOnHit::PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos,VECTOR& _moveDiff,
	ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans):
	CharacterOnHitBase(_chara,_movedPos,_moveDiff,_action,_colParam,_trans)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_={
		{ TAG::ENEMY1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); } },
		{ TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); } },
		{ TAG::ROCK, [this](const std::weak_ptr<Collider>_hitCol) {CollRock(_hitCol); } },
	};
}

PlayerOnHit::~PlayerOnHit(void)
{
	colUpdates_.clear();
	colParam_.clear();
}

void PlayerOnHit::Load(void)
{
}

void PlayerOnHit::Init(void)
{
	hitPoint_ = {};
	bodyRadius_ = Player::CAP_RADIUS;
	movedPos_ = Utility3D::VECTOR_ZERO;
}

void PlayerOnHit::CollChara(const std::weak_ptr<Collider> _hitCol)
{
	//相手のタグの取得
	const CharacterBase& parentChara = _hitCol.lock()->GetParentCharacter();
	std::set<Collider::TAG> tags = _hitCol.lock()->GetTags();

	//攻撃のタグを見つける
	const auto it = std::find(tags.begin(), tags.end(), Collider::TAG::NML_ATK);

	isHitTarget_ = true;

	//攻撃の当たり判定の場合は無視
	if (it != tags.end())return;

	//それぞれのカプセルの取得
	Geometry& myCap = colParam_[TAG_PRIORITY::BODY]->GetGeometry();
	Geometry& hitCap = _hitCol.lock()->GetGeometry();

	//自分の座標
	VECTOR myPos = charaObj_.GetTransform().pos;
	const VECTOR hitCharaPos = parentChara.GetTransform().pos;

	//お互いの距離をとる
	float dis = static_cast<float>(Utility3D::Distance(myPos, hitCharaPos));
	float minDist = myCap.GetRadius() + hitCap.GetRadius();

	//お互いの距離が離れていたら押し出さない
	if (dis > minDist)return;

	//押し出し量の計算
	float pushPow = abs(minDist - dis);

	//押し出す方向ベクトルの計算
	VECTOR vec = Utility3D::GetMoveVec(parentChara.GetTransform().pos,charaObj_.GetTransform().pos );

	//Y成分はいらない
	vec.y = 0.0f;

	//押し出し
	trans_.pos = VAdd(moveDiff_, VScale(vec, pushPow));
}

void PlayerOnHit::CollNormalAttack(const std::weak_ptr<Collider> _hitCol)
{
	//相手の親キャラクターの取得
	auto& parentChara = _hitCol.lock()->GetParentCharacter();

	//ダメージ判定の取得
	bool getIsDam = parentChara.GetIsDamage();
	if (getIsDam)return;

	//相手のタグの取得
	auto tag = _hitCol.lock()->GetParentCharacter().GetCharaTag();

	//ダメージを与えたことを知らせる
	parentChara.SetIsDamage();

	//ダメージを計算
	float damage = parentChara.GetMainAction().GetAtkPoint();

	//ダメージ処理
	charaObj_.Damage(static_cast<int>(damage));
	
	//攻撃中に敵の攻撃を食らった場合、カードを消費する
	charaObj_.SetUsedCard();

	//ヒットSE再生
	SoundManager::GetInstance().Play(ResourceManager::SRC::ENEMY_HIT_SE, SoundManager::PLAYTYPE::BACK);

	//のけぞり状態へ
	action_.ChangeAction(ActionController::ACTION_TYPE::REACT);
	
}

void PlayerOnHit::CollRock(const std::weak_ptr<Collider> _hitCol)
{
	//岩の取得
	auto& rock = dynamic_cast<EnemyRock&>(_hitCol.lock()->GetParent());

	//ダメージを与えていた場合、処理を抜ける
	if (rock.GetIsDamaged())return;

	//ダメージを計算
	float damage = rock.GetAttackPow();

	//ダメージを与える
	charaObj_.Damage(damage);

	//のけぞり時間セット
	rock.SetIsDamaged();

	//ヒットSE再生
	SoundManager::GetInstance().Play(ResourceManager::SRC::ENEMY_HIT_SE, SoundManager::PLAYTYPE::BACK);

	//のけぞり状態へ
	action_.ChangeAction(ActionController::ACTION_TYPE::REACT);
}

#ifdef _DEBUG
void PlayerOnHit::DrawDebug(void)
{

}
#endif // _DEBUG


