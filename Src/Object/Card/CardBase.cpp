#include<DxLib.h>
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"
#include "CardBase.h"

CardBase::CardBase(const CARD_STATUS _status) :
	status_(_status),
	isWin_(false),
	isUsed_(false),
	atkCardImg_(UtilityCommon::INITIAL_HANDLE),
	cardPos_(),
	cardScl_(CARD_SCALE),
	numPos_(),
	cardNoImg_(nullptr)
{
}

CardBase::~CardBase(void)
{
}

void CardBase::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImg_ = res.Load(ResourceManager::SRC::NUMBERS_IMGS).handleIds_;
}

void CardBase::Init(void)
{
}

void CardBase::Update(void)
{
}

void CardBase::Draw(void)
{
}
