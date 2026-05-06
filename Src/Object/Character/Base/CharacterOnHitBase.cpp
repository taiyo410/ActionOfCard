#include "../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../../../Utility/Utility3D.h"
#include "../Object/Character/Player/ActionController.h"
#include "CharacterOnHitBase.h"

CharacterOnHitBase::CharacterOnHitBase(CharacterBase& _chara, VECTOR& _movedPos,VECTOR& _moveDiff,
	ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans):
	charaObj_(_chara),
	movedPos_(_movedPos),
	moveDiff_(_moveDiff),
	colParam_(_colParam),
	action_(_action),
	trans_(_trans),
	hitPoint_(),
	isHitTarget_(false),
	bodyRadius_()
{
}

CharacterOnHitBase::~CharacterOnHitBase(void)
{
}

void CharacterOnHitBase::OnHitUpdate(const std::weak_ptr<Collider> _hitCol)
{
	for (const auto tag : _hitCol.lock()->GetTags())
	{
		colUpdates_[tag](_hitCol);
	}
}

void CharacterOnHitBase::InitHit(void)
{
	hitPoint_ = {};
	isHitTarget_ = false;
}

void CharacterOnHitBase::CollNone(void)
{
	//何もしない
}

void CharacterOnHitBase::CollStage(const std::weak_ptr<Collider> _hitCol)
{
	HitModelCommon(_hitCol);
}

void CharacterOnHitBase::HitModelCommon(const std::weak_ptr<Collider> _hitCol)
{

	const Geometry& hitModel = _hitCol.lock()->GetGeometry();

	//当たったモデルの情報を取得
	//移動後座標と現在座標で早い移動速度でも対応させる
	VECTOR hitPos = hitModel.GetHitLineInfo().HitPosition;

	//移動後と移動前のコライダ
	auto& moveLineCol = colParam_[ObjectBase::TAG_PRIORITY::MOVE_LINE];

	//上下を引いたラインのコライダ(接地)
	auto& upDownLine = colParam_[ObjectBase::TAG_PRIORITY::UPDOWN_LINE];

	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[ObjectBase::TAG_PRIORITY::BODY];

	if (moveLineCol->IsHit())
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos.y + POSITION_OFFSET;

		hitPoint_.isDown = true;
		//現在座標の更新
		trans_.pos = movedPos_;
		return;
	}

	//プレイヤーの接地
	if (upDownLine->IsHit())
	{
		//ライン情報の取得
		Geometry& upDown = upDownLine->GetGeometry();
		VECTOR hitLinePos = upDown.GetHitLineInfo().HitPosition;

		//座標が当たっているライン座標より上のとき、地面と当たる
		if (movedPos_.y > hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y /*+ bodyRadius_*/ + POSITION_OFFSET;
			hitPoint_.isDown = true;
		}

		//移動後座標が当たっているライン座標より下の場合、上に押し出す
		if (movedPos_.y < hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y + POSITION_OFFSET;
			hitPoint_.isOverHead = true;

		}
	}

	//移動後座標を一回格納し、移動前をとる
	Transform trans = Transform(trans_);
	trans.pos = movedPos_;
	trans.Update();

	//プレイヤーの体の球が当たったら
	if (bodyShere->IsHit())
	{
		auto& hitInfo = hitModel.GetHitInfo();
		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			hitPoint_.isSide = true;
			//当たっている部分の情報を取得
			auto hit = hitInfo.Dim[i];

			//一定回数の押し出し処理をする
			for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
			{
				int pHit = HitCheck_Sphere_Triangle(trans.pos, bodyRadius_
					, hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					VECTOR normal = hit.Normal;
					//y座標を抜いて押しだす
					normal.y = 0.0f;
					movedPos_ = VAdd(movedPos_, VScale(normal, HIT_NORMAL_OFFSET));

					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
	}

	//移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
}

