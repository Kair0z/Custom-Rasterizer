#pragma once

#include "UISheet.h"

class Master;

namespace PremadeSheets
{
	UISheet* EmptySheet(const std::string& name = "mySheet");

	UISheet* ButtonSheet(Master* pMaster);
	UISheet* SliderSheet(Master* pMaster);
}
