#include "../pch.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Character/Player/ActionController.h"
#include "../Object/Character/Base/LogicBase.h"
#include "../Object/Character/Base/CharacterOnHitBase.h"
#include "../Object/Common/EffectController.h"
#include"../Object/Card/CardDeck.h"
#include"../Object/Card/CardUIBase.h"
#include "../Object/Stage.h"
#include "../../../Utility/Utility3D.h"
#include "../../Common/Geometry/Capsule.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/Geometry/Line.h"
#include "../Object/Character/Enemy/EnemyRock.h"
#include "../Object/ObjectBase.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void) :
	movedPos_(Utility3D::VECTOR_ZERO),
	moveDiff_(Utility3D::VECTOR_ZERO),
	soundMng_(SoundManager::GetInstance()),
	isMoveable_(true),
	updatePhase_(UPDATE_PHASE::NONE),
	uiMng_(UIManager::GetInstance()),
	hitStopFrame_(HIT_STOP_FRAME),
	isEndClearDirect_(false),
	footSEDisCount_(0.0f)
{
	changeUpdate_ = {
		{UPDATE_PHASE::NONE,[this]() {ChangeUpdateNone(); }},
		{UPDATE_PHASE::NORMAL,[this]() {ChangeUpdateNormal(); }},
		{UPDATE_PHASE::DIRECTION, [this]() {ChangeUpdateDirection(); }},
		{UPDATE_PHASE::CLEAR_DIRECTION, [this]() {ChangeUpdateClearDirection(); }},
		{UPDATE_PHASE::OVER_DIRECTION, [this]() {ChangeUpdateOverDirection(); }},
		{UPDATE_PHASE::HIT_STOP,[this]() {ChangeUpdateHitStop(); } }
	};
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::Update(void)
{
	phazeUpdate_();
}

void CharacterBase::MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius)
{
	//当たり判定が存在したら削除する
	if (IsAliveCollider(_charaTag, _attackTag))return;
	std::unique_ptr<Sphere>sphere = std::make_unique<Sphere>(_atkPos, _radius);

	MakeCollider(TAG_PRIORITY::ATK_SPHERE,{ _charaTag,_attackTag }, std::move(sphere),{Collider::TAG::STAGE});
}

void CharacterBase::UpdateAttackCol(const float _radius)
{
	//引数の半径を攻撃の球に設定する
	Sphere& sphere=dynamic_cast<Sphere&>(collider_[TAG_PRIORITY::ATK_SPHERE]->GetGeometry());
	sphere.SetRadius(_radius);
}

void CharacterBase::DeleteAttackCol(const Collider::TAG& _charaTag, const Collider::TAG& _attackCol)
{
	if (!IsAliveCollider(_charaTag, _attackCol))return;
	DeleteCollider(TAG_PRIORITY::ATK_SPHERE);
}

void CharacterBase::UpdatePost(void)
{
	//移動後座標の更新
	movedPos_ = VAdd(trans_.pos, action_->GetMovePow());

	//移動量ラインの更新
	VECTOR moveVec = VSub(movedPos_, trans_.pos);
	moveVec.y -= MOVE_LINE_Y_OFFSET;
	if (moveVec.x != 0.0f || moveVec.y != MOVE_LINE_Y_CHECK_VALUE || moveVec.z != 0.0f)
	{
		Line& moveLine = dynamic_cast<Line&>(collider_[TAG_PRIORITY::MOVE_LINE]->GetGeometry());
		moveLine.SetLocalPosPoint1(Utility3D::VECTOR_ZERO);
		moveLine.SetLocalPosPoint2(moveVec);
	}

	//当たり判定をする前に初期化する
	onHit_->InitHit();

	//移動制限
	MoveLimit(Stage::STAGE_POS, { Stage::STAGE_SIZE,0.0f, Stage::STAGE_SIZE });

	//移動前の座標を格納する
	moveDiff_ = trans_.pos;

	//移動
	trans_.pos = movedPos_;
}

void CharacterBase::LoadStatus(void)
{
	//jsonロード
	nlohmann::json j = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;

	std::string statusPath = "";

	characterType_ == CHARACTER_TYPE::PLAYER ? statusPath = PLAYER_STATUS_DATA 
												: statusPath = ENEMY_STATUS_DATA;

	//データを格納
	for (const auto& data : j[statusPath])
	{
		if(data.contains("HP"))
		{ 
			maxStatus_.hp = data["HP"]; 
		}
		if (data.contains("SPD")) 
		{
			maxStatus_.speed = data["SPD"];
		}
	}

	//現在ステータスを最大値にセット
	status_ = maxStatus_;
}

void CharacterBase::MoveLimit(const VECTOR& _stagePos,const VECTOR& _stageSize)
{
	//カプセルを考慮したステージの制限サイズ
	VECTOR subRadiusSize={_stageSize.x- capRadius_,0.0f,_stageSize.z- capRadius_};

	//センターサイズ
	VECTOR sizeHalf = VScale(subRadiusSize, 0.5f);

	//制限
	VECTOR limit = VAdd(_stagePos, sizeHalf);

	//移動ベクトル
	VECTOR moveVec = Utility3D::GetMoveVec(trans_.pos, movedPos_);

	//カプセル関係を考慮した移動座標
	VECTOR addPos = VScale(moveVec, capRadius_);

	//制限座標
	VECTOR limitPos = VAdd(trans_.pos, addPos);

	//押し出しベクトル
	VECTOR pushVec = Utility3D::GetMoveVec(movedPos_, moveDiff_);
	pushVec.y = 0.0f;
	VECTOR pushPow = VScale(pushVec, action_->GetSpd());

	//移動制限
	if (limitPos.x >= limit.x)
	{
		movedPos_.x = limit.x + pushPow.x;
	}
	if (limitPos.x <= -limit.x)
	{
		movedPos_.x = -limit.x + pushPow.x;
	}
	if (limitPos.z >= limit.z)
	{
		movedPos_.z = limit.z + pushPow.z;
	}
	if (limitPos.z <= -limit.z)
	{
		movedPos_.z = -limit.z + pushPow.z;
	}
}

void CharacterBase::SetUsedCard(void)
{
	uiMng_.GetCardUI(characterType_).ChangeReactActionCard();
	deck_->EraseHandCard();
}

void CharacterBase::ChangeUpdatePhase(const UPDATE_PHASE _phase)
{
	if (updatePhase_ == _phase)return;
	updatePhase_ = _phase;
	changeUpdate_[updatePhase_]();
}

void CharacterBase::ChangeDirectToNormal(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	phazeUpdate_ = [this]() {UpdateNormal(); };
}

void CharacterBase::ChangeUpdateClearDirection(void)
{
	phazeUpdate_ = [this]() {UpdateClearDirection(); };
}

void CharacterBase::ChangeUpdateOverDirection(void)
{
	phazeUpdate_ = [this]() {UpdateOverDirection(); };
}

void CharacterBase::MoveDirFromInput(void)
{
}

void CharacterBase::Rotate(void)
{
	if (charaRot_.stepRotTime_ <= 0.0f)return;
	charaRot_.stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	charaRot_.playerRotY_ = Quaternion::Slerp(
		charaRot_.playerRotY_, charaRot_.goalQuaRot_, (TIME_ROT - charaRot_.stepRotTime_) / TIME_ROT);
}

void CharacterBase::Damage(const int _dam)
{
	//ダメージを受ける前にUI補間するためのpreHpを計算
	HP_DATA hpData = {};

	//減らす前のhpを入れる
	hpData.preHpPer = status_.hp / maxStatus_.hp;

	//ダメージ分hpを減らす
	status_.hp -= _dam;

	//減らした後のHPを入れる
	hpData.hpPer = status_.hp / maxStatus_.hp;

	//UIマネージャから更新
	uiMng_.RefreshHpUI(characterType_, hpData);
}

const bool CharacterBase::GetIsDamage(void) const
{
	return action_->GetMainAction().GetIsDamage();
}

VECTOR CharacterBase::GetCharaCenterPos(void) const
{
	return collider_.at(ObjectBase::TAG_PRIORITY::BODY)->GetGeometry().GetCenter();
}

void CharacterBase::SetIsDamage(void)
{
	action_->GetMainAction().SetIsDamage();
}

void CharacterBase::SetFlinchCnt(const float _flichCnt)
{
	action_->SetFlinchCnt(_flichCnt);
}

const CardActionBase::CARD_ACT_TYPE& CharacterBase::GetCardAction(void) const
{
	return action_->GetCardAction();
}

const ActionBase& CharacterBase::GetMainAction(void) const
{
	return action_->GetMainAction();
}

const CharacterOnHitBase::HIT_POINT& CharacterBase::GetHitPoint(void) const
{
	return onHit_->GetHitPoint();
}

void CharacterBase::SetLogicTargetCharacter(std::shared_ptr<CharacterBase> _targetChara)
{
	logic_->SetTargetCharacter(_targetChara);
}

void CharacterBase::AddEnemyRock(const int _num, VECTOR& _atkPos)
{
	for (int i = 0; i < _num; i++)
	{
		std::unique_ptr<EnemyRock> rock = std::make_unique<EnemyRock>(i, _atkPos);
		rock_.emplace_back(std::move(rock));
	}
}

void CharacterBase::LoadEnemyRock(void)
{
	if (rock_.empty())return;
	for (auto& rock : rock_)
	{
		rock->Load();
	}
}

void CharacterBase::DeleteEnemyRockCol(void)
{
	if (rock_.empty())return;
	for (auto& rock : rock_)
	{
		rock->DeleteRockCollider();
	}
}

void CharacterBase::SetIsAliveEnemyRock(const bool _isAlive)
{
	if (rock_.empty())return;
	for (auto& rock : rock_)
	{
		rock->SetIsAlive(_isAlive);
		if (_isAlive)
		{
			rock->Init();
		}
	}
}

void CharacterBase::EnemyRockUpdate(void)
{
	if (rock_.empty())return;
	for (auto& rock : rock_)
	{
		rock->Update();
	}
}
const bool CharacterBase::GetIsHitTarget(void) const
{
	return onHit_->GetIsHitTarget();
}

void CharacterBase::UpdateNone(void)
{
	//何もしない
}

void CharacterBase::UpdateHitStop(void)
{
	if (--hitStopFrame_ > 0)return;
	hitStopFrame_ = HIT_STOP_FRAME;
	ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
}

void CharacterBase::ChangeUpdateNone(void)
{
	phazeUpdate_ = [this]() {UpdateNone(); };
}

void CharacterBase::ChangeUpdateNormal(void)
{
	phazeUpdate_ = [this]() {UpdateNormal(); };
}

void CharacterBase::ChangeUpdateDirection(void)
{
	phazeUpdate_ = [this]() { UpdateDirection(); };
}

void CharacterBase::ChangeUpdateHitStop(void)
{
	phazeUpdate_ = [this]() { UpdateHitStop(); };
}
