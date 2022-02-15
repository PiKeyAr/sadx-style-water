#include <SADXModLoader.h>
#include <IniFile.hpp>

// INI settings
bool SADXWater_EmeraldCoast;
bool SADXWater_StationSquare;
bool SADXWater_MysticRuins;
bool SADXWater_EggCarrier;
bool SADXWater_Past;

// Pointers and functions
DataPointer(int, FramerateSetting, 0x389D7DC); // No name in symbols

void EmeraldCoastWater_Init();
void EmeraldCoastWater_OnFrame();

// Common water data
int SADXStyleWaterAnimationFrame = 0;
NJS_TEXNAME textures_sadxwater[27];
NJS_TEXLIST texlist_sadxwater = { arrayptrandlength(textures_sadxwater) };
PVMEntry SADXWaterPVMList = { "SADXWTR", &texlist_sadxwater };

// Emerald Coast green sea texture
NJS_TEXLIST texlist_beachsea = { &textures_sadxwater[15], 1 };

void __cdecl StationSquare_OceanDraw_SADXStyle(stcWaterSurface* pu32SaltStat)
{
	if (ssStageNumber == LevelIDs_StationSquare)
	{
		___njFogDisable();
		njSetTexture(&texlist_sadxwater);
		njSetZCompare(3u);
		njSetZUpdateMode(0);
		njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		int textureID = SeqGetTime() + 21;
		if (ssActNumber != 4 && pu32SaltStat->u8StripArrayNo) // Sewer texture
			textureID = 24;
		njDrawTexture3DExStart(1);
		njPushMatrix(0);
		float shift = njSin(gu32LocalCnt << 6);
		njTranslate(0,
			(pu32SaltStat->f32x0 + (shift * pu32SaltStat->f32TransScale)),
			pu32SaltStat->f32y0,
			(pu32SaltStat->f32z0 + (shift * pu32SaltStat->f32TransScale)));
		njPushMatrix(0);
		float xb = (shift * pu32SaltStat->f32TransScale) * 0.5f;
		njTranslate(0, xb, -1.0f, xb);
		njSetTextureNum(textureID);
		// Sea texture
		if (pu32SaltStat->u8WrapX)
		{
			for (int v9 = 0; v9 < pu32SaltStat->u8WrapX; v9++)
			{
				njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
				njTranslate(0, pu32SaltStat->f32WrapXZ, 0.0f, 0.0f);
			}
		}
		njPopMatrix(1u);
		// Waves
		njPushMatrix(0);
		njSetTextureNum(SADXStyleWaterAnimationFrame);
		int v10 = 0;
		if (pu32SaltStat->u8WrapX)
		{
			for (int v10 = 0; v10 < pu32SaltStat->u8WrapX; v10++)
			{
				njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
				njTranslate(0, pu32SaltStat->f32WrapXZ, 0.0f, 0.0f);
			}
		}
		njPopMatrix(1u);
		njPopMatrix(1u);
		npSetZCompare();
		njSetZUpdateMode(1u);
		___njFogEnable();
	}
}

void __cdecl MysticRuins_OceanDraw_SADXStyle(stcWaterSurface* pu32SaltStat)
{
	if ((ssStageNumber == LevelIDs_MysticRuins && !ssActNumber) || ssStageNumber == LevelIDs_EggHornet)
	{
		___njFogDisable();
		njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njSetTexture(&texlist_sadxwater);
		njSetZCompare(3u);
		njSetZUpdateMode(0);
		njDrawTexture3DExStart(1);
		njPushMatrix(0);
		float xa = njSin(gu32LocalCnt << 6) * pu32SaltStat->f32TransScale;
		njTranslate(0, xa + pu32SaltStat->f32x0, pu32SaltStat->f32y0, xa + pu32SaltStat->f32z0);
		if (pu32SaltStat->u8WrapX)
		{
			float xb = xa * 0.5f;
			for (int v6 = 0; v6 < pu32SaltStat->u8WrapX; v6++)
			{
				njPushMatrix(0);
				njTranslate(0, xb, 0.0f, xb);
				njSetTextureNum(18);
				njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
				njPopMatrix(1u);
				njPushMatrix(0);
				njTranslate(0, 0.0f, 1.0f, 0.0f);
				njSetTextureNum(SADXStyleWaterAnimationFrame);
				njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
				njPopMatrix(1u);
				njTranslate(0, pu32SaltStat->f32WrapXZ, 0.0f, 0.0f);
			}
		}
		njPopMatrix(1u);
		npSetZCompare();
		njSetZUpdateMode(1u);
		___njFogEnable();
	}
}

void __cdecl EggCarrier_OceanDraw_SADXStyle(stcWaterSurface* pu32SaltStat)
{
	___njFogDisable();
	___njClipZ(gClipSky.f32Near, gClipSky.f32Far);
	njSetTexture(&texlist_sadxwater);
	njPushMatrix(0);
	if (camera_twp)
		njTranslate(0, camera_twp->pos.x, 0.0f, camera_twp->pos.z);
	njScale(0, 2.0f, 1.0f, 2.0f);
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
	njSetZUpdateMode(0);
	njSetZCompare(3u);
	njDrawTexture3DExStart(1);
	njPushMatrix(0);
	float v3 = njSin(gu32LocalCnt << 6) * pu32SaltStat->f32TransScale;
	njTranslate(0, v3 + pu32SaltStat->f32x0, pu32SaltStat->f32y0, v3 + pu32SaltStat->f32z0);
	if (pu32SaltStat->u8WrapX)
	{
		float xz = v3 * 0.5f;
		for (int v7 = 0; v7 < pu32SaltStat->u8WrapX; v7++)
		{
			njPushMatrix(0);
			njTranslate(0, xz, 0.0f, xz);
			njSetTextureNum(SADXStyleWaterAnimationFrame);
			njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
			njPopMatrix(1u);
			njPushMatrix(0);
			njTranslate(0, 0.0f, 2.5f, 0.0f);
			njSetTextureNum(17);
			njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
			njPopMatrix(1u);
			njTranslate(0, pu32SaltStat->f32WrapXZ, 0.0f, 0.0f);
		}
	}
	njPopMatrix(1u);
	npSetZCompare();
	njSetZCompare(1u);
	njPopMatrix(1u);
	___njFogEnable();
	___njClipZ(gClipMap.f32Near, gClipMap.f32Far);
	___njFogEnable();
}


void __cdecl Past_OceanDraw_SADXStyle(stcWaterSurface* pu32SaltStat)
{
	int textureID = ssActNumber == 1 ? 19 : 20;
	___njFogDisable();
	njSetTexture(&texlist_sadxwater);
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
	njSetZCompare(3u);
	njSetZUpdateMode(0);
	njDrawTexture3DExStart(1);
	njPushMatrix(0);
	float xa = njSin(gu32LocalCnt << 6) * pu32SaltStat->f32TransScale;
	njTranslate(0, xa + pu32SaltStat->f32x0, pu32SaltStat->f32y0, xa + pu32SaltStat->f32z0);
	njPushMatrix(0);
	float xb = xa * 0.5f;
	njTranslate(0, xb, 0.0f, xb);
	njSetTextureNum(SADXStyleWaterAnimationFrame);
	if (pu32SaltStat->u8WrapX)
	{
		for (int v8 = 0; v8 < pu32SaltStat->u8WrapX; v8++)
		{
			njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
			njTranslate(0, pu32SaltStat->f32WrapXZ, 0.0f, 0.0f);
		}
	}
	njPopMatrix(1u);
	njPushMatrix(0);
	njTranslate(0, 0.0f, 1.0f, 0.0f);
	njSetTextureNum(textureID);
	if (pu32SaltStat->u8WrapX)
	{
		for (int v9 = 0; v9 < pu32SaltStat->u8WrapX; v9++)
		{
			njDrawTexture3DExSetData((NJS_TEXTURE_VTX*)gsaStripPool[pu32SaltStat->u8StripArrayNo], 4 * pu32SaltStat->u8WrapZ);
			njTranslate(0, pu32SaltStat->f32WrapXZ, 0.0f, 0.0f);
		}
	}
	njPopMatrix(1u);
	njPopMatrix(1u);
	npSetZCompare();
	njSetZUpdateMode(1u);
	___njFogEnable();
}

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Config stuff
		const std::string s_path(path);
		const std::string s_config_ini(s_path + "\\config.ini");
		const IniFile* const config = new IniFile(s_config_ini);
		SADXWater_EmeraldCoast = config->getBool("General", "EmeraldCoast", true);
		SADXWater_StationSquare = config->getBool("General", "StationSquare", true);
		SADXWater_MysticRuins = config->getBool("General", "MysticRuins", true);
		SADXWater_EggCarrier = config->getBool("General", "EggCarrier", true);
		SADXWater_Past = config->getBool("General", "Past", true);

		helperFunctions.RegisterCommonObjectPVM(SADXWaterPVMList);
		if (SADXWater_EmeraldCoast)
			EmeraldCoastWater_Init();
		if (SADXWater_StationSquare)
		{
			WriteData((float*)0x006313F1, -1500.012f); // For DC Conversion
			WriteJump(StationSquare_OceanDraw, StationSquare_OceanDraw_SADXStyle);
		}
		if (SADXWater_MysticRuins)
		{
			WriteJump(MysticRuins_OceanDraw, MysticRuins_OceanDraw_SADXStyle);
			WriteJump(EggHornet_OceanDraw, MysticRuins_OceanDraw_SADXStyle);
		}
		if (SADXWater_EggCarrier)
		{
			WriteJump(EggCarrier_OceanDraw, EggCarrier_OceanDraw_SADXStyle);
			WriteJump(E101Mk2_OceanDraw, EggCarrier_OceanDraw_SADXStyle);
			WriteJump(Zero_OceanDraw, EggCarrier_OceanDraw_SADXStyle);
		}
		if (SADXWater_Past)
		{
			WriteData((float*)0x00542A38, -2800.012f); // For DC Conversion
			WriteJump((void*)0x542850, Past_OceanDraw_SADXStyle);
		}
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		if (ssGameMode == 16)
			return;
		if ((FramerateSetting < 2 && gu32GameCnt % 4 == 0) || (FramerateSetting == 2 && gu32GameCnt % 2 == 0) || FramerateSetting > 2)
		{
			SADXStyleWaterAnimationFrame++;
			if (SADXStyleWaterAnimationFrame > 14)
				SADXStyleWaterAnimationFrame = 0;
		}
		if (ssStageNumber == LevelIDs_EmeraldCoast)
			EmeraldCoastWater_OnFrame();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}