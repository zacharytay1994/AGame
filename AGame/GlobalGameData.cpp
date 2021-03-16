#include "GlobalGameData.h"

GlobalGameData& GlobalGameData::Instance()
{
	static GlobalGameData instance;
	return instance;
}
