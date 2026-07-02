#include "pch.h"
#include "Utility/Utility3D.h"
#include "Utility/UtilityJson.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/UIManager.h"
#include "Renderer/ModelMaterial.h"
#include "Renderer/ModelRenderer.h"
#include "Object/Common/AnimationController.h"
#include "Object/Common/EffectController.h"
#include "Object/Common/Geometry/Capsule.h"
#include "Object/Common/Geometry/Sphere.h"
#include "Object/Common/Geometry/Line.h"
#include "Object/ObjectBase.h"
#include "Object/Stage.h"
#include "Object/Card/CardDeck.h"
#include "Object/Card/CardUIBase.h"
#include "Object/Character/Enemy/EnemyRock.h"
#include "Object/Character/Action/ActionController.h"
#include "Object/Character/Base/LogicBase.h"
#include "Object/Character/Base/CharacterOnHitBase.h"
#include "./CharacterBase.h"

CharacterBase::CharacterBase(void) :
	movedPos_(Utility3D::VECTOR_ZERO),
	moveDiff_(Utility3D::VECTOR_ZERO),
	soundMng_(SoundManager::GetInstance()),
	isMoveable_(true),
	phase_(UPDATE_PHASE::NONE),
	uiMng_(UIManager::GetInstance()),
	hitStopFrame_(HIT_STOP_FRAME),
	isEndClearDirect_(false),
	animSpdScl_(UtilityCommon::RATIO_MAX),
	characterType_(CHARACTER_TYPE::MAX)
{
	changePhase_ = {
		{UPDATE_PHASE::NONE,[this]() {ChangeUpdateNone(); }},
		{UPDATE_PHASE::NORMAL,[this]() {ChangeUpdateNormal(); }},
		{UPDATE_PHASE::DIRECTION, [this]() {ChangeUpdateDirection(); }},
		{UPDATE_PHASE::CLEAR_DIRECTION, [this]() {ChangeUpdateClearDirection(); }},
		{UPDATE_PHASE::OVER_DIRECTION, [this]() {ChangeUpdateOverDirection(); }},
		{UPDATE_PHASE::HIT_STOP,[this]() {ChangeUpdateHitStop(); } }
	};

	animStrTable_ = {
		{"Idle", ANIM_TYPE::IDLE},
		{"Run", ANIM_TYPE::RUN},
		{"React", ANIM_TYPE::REACT},
		{"Dodge", ANIM_TYPE::DODGE},
		{"Death", ANIM_TYPE::DEATH},
		{"React", ANIM_TYPE::REACT},
		{"Attack_1_Middle", ANIM_TYPE::ATTACK_1_MIDDLE},
		{"Attack_1_Short", ANIM_TYPE::ATTACK_1_SHORT},
		{"Attack_2", ANIM_TYPE::ATTACK_2},
		{"Attack_3", ANIM_TYPE::ATTACK_3},
		{"FireMagic", ANIM_TYPE::MAGIC_FIRE},
		{"Reload", ANIM_TYPE::CARD_RELOAD},
		{"StompAttack", ANIM_TYPE::STOMP_ATK},
		{"JumpAttack", ANIM_TYPE::JUMP_ATK},
		{"Rush_Atk", ANIM_TYPE::RUSH_ATK},
		{"Roar", ANIM_TYPE::ROAR_ATK},
		{"Revolution", ANIM_TYPE::REVOLUTION},
	};

	loadDataFuncTable_ = {
		{"Status",[this](const nlohmann::json& _data) {LoadStatus(_data); }},
		{"ModelData",[this](const nlohmann::json& _data) {
			LoadModelDataCommon(_data); 
			LoadModelDataCharacter(_data);
		}},
		{"BattleStartPos",[this](const nlohmann::json& _data) {LoadBattleStartPos(_data); }},
	};

	animCtrl_ = std::make_unique<AnimationController>();
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::Load(void)
{
	LoadCommon();
	LoadCharacter();
}

void CharacterBase::LoadCommon(void)
{
	//共通のパラメータのロード
	LoadCommonData();

	material_ = std::make_unique<ModelMaterial>(
		ResourceManager::SRC::CHARACTER_MODEL_VS,
		ResourceManager::SRC::CHARACTER_MODEL_PS);

	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId, *material_);

	//当たり判定の生成
	MakeColliderGeometry();

	//使用するアクションを追加
	actionCtrl_->AddAction();

	//Jsonからアクションごとのデータのロード
	LoadActionData([this](const ACTION_LOAD_DATA& _actionVar)
		{
			//アクションコントローラーの全行動クラスに通知
			actionCtrl_->AnimLoadNotify(_actionVar);
			LoadCharacterActionDataCallBack(_actionVar);
		});

	//アクションのロード
	actionCtrl_->Load();

	//衝突後処理のロード
	onHit_->Load();
}

void CharacterBase::LoadCommonData(void)
{
	const auto& jsonData = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;
	for (const auto& [key,data] : jsonData[objectName_].items())
	{
		//配列内になければ飛ばす
		auto it = loadDataFuncTable_.find(key);
		if (it == loadDataFuncTable_.end())continue;

		loadDataFuncTable_[key](data);
	}
}

void CharacterBase::InitCommon(void)
{
	trans_.scl = { modelScl_,modelScl_,modelScl_ };
	trans_.quaRotLocal =
		Quaternion::Euler({ UtilityCommon::Deg2RadF(localDeg_.x)
			, UtilityCommon::Deg2RadF(localDeg_.y)
			, UtilityCommon::Deg2RadF(localDeg_.z) });
	trans_.pos = battleStartPos_;
	trans_.localPos = localPos_;

	//入力
	logic_->Init();
	actionCtrl_->Init();
	deck_->Init();
	//更新
	trans_.Update();
}

void CharacterBase::UpdateNormalCommon(void)
{
	//ロジックによる操作の受付
	AcceptLogicControl();
}

void CharacterBase::UpdateDirectionCommon(void)
{
	//方向の更新
	actionCtrl_->Update();
}

void CharacterBase::UpdateClearDirectionCommon(void)
{
}

void CharacterBase::UpdateOverDirectionCommon(void)
{
}

void CharacterBase::Init(void)
{
	InitCommon();
	InitCharacter();
}

void CharacterBase::Update(void)
{
	updatePhase_();

	//アニメーションの更新
	animCtrl_->Update(animSpdScl_);

	//Transformの更新
	trans_.quaRot = charaRot_.playerRotY_;
	trans_.Update();
}

void CharacterBase::Draw(void)
{
	DrawCommon();
	DrawCharacter();
}

void CharacterBase::DrawCommon(void)
{
	//通常描画
	//MV1DrawModel(trans_.modelId);
	renderer_->Draw();
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

void CharacterBase::LookAtTargetVec(void)
{
	VECTOR targetDir = logic_->GetLookAtTargetDir();
	charaRot_.dir_ = targetDir;
}

void CharacterBase::UpdatePost(void)
{
	//移動後座標の更新
	movedPos_ = VAdd(trans_.pos, actionCtrl_->GetMovePow());

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

void CharacterBase::LoadStatus(const nlohmann::json& _data)
{
	//データを格納
	maxStatus_.hp = _data.value("hp", 0.0f);
	maxStatus_.speed = _data.value("speed", 0.0f);

	//現在ステータスを最大値にセット
	status_ = maxStatus_;
}

void CharacterBase::LoadModelDataCommon(const nlohmann::json& _data)
{
	//データを格納
	//使用モデル
	std::string modelStr = _data.value("model", "");
	useModelSrc_ = resMng_.GetSrcFromString(modelStr);
	trans_.SetModel(resMng_.LoadModelDuplicate(useModelSrc_));

	//ローカル角度
	localDeg_ = UtilityJson::GetLoadVector3("localDegree", _data);

	//ローカル座標
	localPos_ = UtilityJson::GetLoadVector3("localPos", _data);

	//モデルの大きさ
	modelScl_ = _data.value("scale", 0.0f);

	//腰のフレーム番号
	spineFrameNo_ = _data.value("spineFrameNum", 0);

	//アニメーションに必要なモデル情報を渡す
	animCtrl_->SetModelInfo(trans_.modelId, spineFrameNo_);
}

void CharacterBase::LoadBattleStartPos(const nlohmann::json& _data)
{
	battleStartPos_ = UtilityJson::GetLoadVector3(_data);
}

void CharacterBase::MoveLimit(const VECTOR& _stagePos,const VECTOR& _stageSize)
{
	const float radius = collider_[TAG_PRIORITY::BODY]->GetGeometry().GetRadius();
	Utility3D::MoveLimit(trans_.pos, movedPos_, radius, _stagePos, _stageSize);
}

void CharacterBase::SetUsedCard(void)
{
	uiMng_.GetCardUI(characterType_).ChangeReactActionCard();
	deck_->EraseHandCard();
}

void CharacterBase::ChangeUpdatePhase(const UPDATE_PHASE _phase)
{
	if (phase_ == _phase)return;
	phase_ = _phase;
	changePhase_[phase_]();
}

void CharacterBase::ChangeDirectToNormal(void)
{
	animCtrl_->PlayBlend(static_cast<int>(ANIM_TYPE::IDLE),idleAnim_);
	updatePhase_ = [this]() {UpdateNormal(); };
}

void CharacterBase::RegisterDrawableRocks(const std::weak_ptr<EnemyRock> _rock)
{
	//寿命が切れている岩は登録しない
	if (_rock.expired())return;

	//岩を登録する
	drawableRocks_.emplace_back(_rock);
}

void CharacterBase::UnRegisterDrawableRocks(void)
{
	//岩を削除する
	drawableRocks_.clear();
}

void CharacterBase::DrawItem(const std::weak_ptr<ItemBase> _item)
{
	drawableItem_ = _item;
}

void CharacterBase::DeleteItem(void)
{
	drawableItem_.reset();
}

void CharacterBase::ChangeUpdateClearDirection(void)
{
	updatePhase_ = [this]() {UpdateClearDirection(); };
}

void CharacterBase::ChangeUpdateOverDirection(void)
{
	updatePhase_ = [this]() {UpdateOverDirection(); };
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
	return actionCtrl_->GetMainAction().GetIsDamage();
}

VECTOR CharacterBase::GetCharaCenterPos(void) const
{
	return collider_.at(ObjectBase::TAG_PRIORITY::BODY)->GetGeometry().GetCenter();
}

void CharacterBase::SetIsDamage(void)
{
	actionCtrl_->GetMainAction().SetIsDamage();
}

void CharacterBase::SetFlinchCnt(const float _flichCnt)
{
	actionCtrl_->SetFlinchCnt(_flichCnt);
}

const bool CharacterBase::GetIsJumpAtk(void) const
{
	return actionCtrl_->GetMainAction().IsJumpAtk();
}

const ActionBase& CharacterBase::GetMainAction(void) const
{
	return actionCtrl_->GetMainAction();
}

const CharacterOnHitBase::HIT_POINT& CharacterBase::GetHitPoint(void) const
{
	return onHit_->GetHitPoint();
}

const VECTOR CharacterBase::GetCapsuleTop(void)const
{
	return collider_.at(TAG_PRIORITY::BODY)->GetGeometry().GetLocalPosPoint1();
}

const VECTOR CharacterBase::GetCapsuleDown(void)const
{
	return collider_.at(TAG_PRIORITY::BODY)->GetGeometry().GetLocalPosPoint2();
}

void CharacterBase::SetLogicTargetCharacter(std::shared_ptr<CharacterBase> _targetChara)
{
	logic_->SetTargetCharacter(_targetChara);
}

const bool CharacterBase::GetIsHitTarget(void) const
{
	return onHit_->GetIsHitTarget();
}

void CharacterBase::LoadActionData(OnActionDataLoaded _callBack)
{
	//データ読み込み
	nlohmann::json j = resMng_.Load(ResourceManager::SRC::ACTION_DATA).jsonData;

	//キャラクターごとでパスを変える
	std::string statusPath = "";
	characterType_ == CHARACTER_TYPE::PLAYER ? statusPath = PLAYER_STATUS_DATA
		: statusPath = ENEMY_STATUS_DATA;

	const auto& actionData = j[statusPath];

	// アクションごとの使用アニメーションを読み取って
	// アニメーションコントローラーに追加する
	for (const auto& [name, data] : actionData.items())
	{
		ACTION_LOAD_DATA actionLoadData = { name, {}, data };

		//animationのデータがあれば格納する
		if (!data.contains("animation"))continue;

		//アニメーションデータの取得
		const auto& animData = data["animation"];

		////Jsonのリスト名と登録情報が一致しているかを調べる
		////使用アニメーションの取得
		std::string useAnim = animData.value("useAnim", "");
		auto nameIt = animStrTable_.find(name);
		if (nameIt == animStrTable_.end())continue;

		//使用するリソースを取得
		ResourceManager::SRC useSrc = resMng_.GetSrcFromString(useAnim);

		//再生するときのパラメータを格納
		AnimationController::ANIMATION_VARIABLE animVariable = {};
		
		if (useSrc != ResourceManager::SRC::NONE)
		{
			//アニメーションに追加
			animCtrl_->Add(static_cast<int>(nameIt->second), resMng_.LoadModelDuplicate(useSrc));
		}

		//アニメーション速度の取得
		animVariable.speed = animData.value("animSpeed", 0.0f);

		//デタッチスピードの取得
		animVariable.detachSpeed = animData.value("detachSpeed", 0.0f);

		//ループフラグの格納
		animVariable.isLoop = animData.value("isLoop", false);

		//スタートステップの取得
		animVariable.step = animData.value("startStep", 0.0f);

		//終了ステップの取得
		animVariable.totalTime = animData.value("endStep", 0.0f);

		//アクションのロードデータに格納
		actionLoadData.animVariable = animVariable;

		//callbackがあれば処理する
		if (_callBack)
		{
			_callBack(actionLoadData);
		}
	}
}

void CharacterBase::AcceptLogicControl(void)
{
	//ロジックの更新
	logic_->Update();

	//アクションの更新
	actionCtrl_->Update();

	//座標更新
	UpdatePost();
}

void CharacterBase::UpdateNone(void)
{
	//何もしない
}

void CharacterBase::UpdateNormal(void)
{
	UpdateNormalCommon();
	UpdateNormalCharacter();
}

void CharacterBase::UpdateDirection(void)
{
	UpdateDirectionCommon();
	UpdateDirectionCharacter();
}

void CharacterBase::UpdateClearDirection(void)
{
	UpdateClearDirectionCommon();
	UpdateClearDirectionCharacter();
}

void CharacterBase::UpdateOverDirection(void)
{
	UpdateOverDirectionCommon();
	UpdateOverDirectionCharacter();
}

void CharacterBase::UpdateHitStop(void)
{
	if (--hitStopFrame_ > 0)return;
	hitStopFrame_ = HIT_STOP_FRAME;
	ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
}

void CharacterBase::ChangeUpdateNone(void)
{
	updatePhase_ = [this]() {UpdateNone(); };
}

void CharacterBase::ChangeUpdateNormal(void)
{
	updatePhase_ = [this]() {UpdateNormal(); };
}

void CharacterBase::ChangeUpdateDirection(void)
{
	updatePhase_ = [this]() { UpdateDirection(); };
}

void CharacterBase::ChangeUpdateHitStop(void)
{
	updatePhase_ = [this]() { UpdateHitStop(); };
}
