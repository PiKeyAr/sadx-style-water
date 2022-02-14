#include <SADXModLoader.h>
#include "Common.h"

bool SADXWater_EmeraldCoast;
bool SADXWater_StationSquare;
bool SADXWater_MysticRuins;
bool SADXWater_EggCarrier;
bool SADXWater_Past;

void SADXStyleWater_Init(const char* path, const HelperFunctions& helperFunctions);
void SADXStyleWater_OnFrame();

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		SADXWater_StationSquare = SADXWater_EmeraldCoast = SADXWater_MysticRuins = SADXWater_EggCarrier = SADXWater_Past = true;
		SADXStyleWater_Init(path, helperFunctions);
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SADXStyleWater_OnFrame();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}