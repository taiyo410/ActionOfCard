#include "InputManager.h"
#include "DataBank.h"


void DataBank::Init(void)
{
	isFullScreen_ = false;
}

void DataBank::SetIsFullScreen(const bool _isFullScreen)
{
	isFullScreen_ = _isFullScreen;
    ChangeWindowMode(!isFullScreen_);
}

DataBank::DataBank(void):
	isFullScreen_(false)
{

}

DataBank::~DataBank(void)
{
}
