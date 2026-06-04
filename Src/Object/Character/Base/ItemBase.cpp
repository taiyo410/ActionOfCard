#include "../pch.h"
#include "../Object/Common/EffectController.h"
#include "ItemBase.h"

ItemBase::ItemBase(void):
	isDamage_(false)
{
	effect_ = std::make_unique<EffectController>();
}

ItemBase::~ItemBase(void)
{
}
