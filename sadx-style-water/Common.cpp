#include <SADXModLoader.h>
#include "Common.h"

char SADXWaveAnimation = 0;

NJS_TEXNAME textures_sadxwtr_sewers[10];
NJS_TEXLIST texlist_sadxwtr_sewers = { arrayptrandlength(textures_sadxwtr_sewers) };

NJS_TEXNAME textures_sadxwtr_waves[26];
NJS_TEXLIST texlist_sadxwtr_waves = { arrayptrandlength(textures_sadxwtr_waves) };

PVMEntry SADXWaterPVM1_Waves = { "SADXWTR_WAVES", (TexList*)&texlist_sadxwtr_waves };

void EmeraldCoastWater_Init();
void EmeraldCoastWater_OnFrame();

void __cdecl StationSquare_OceanDraw_SADXStyle(OceanData* x)
{
	OceanData* v1; // esi
	Uint32 v2; // edi
	float v6; // st7
	float z; // ST14_4
	float XDist; // ST0C_4
	unsigned int v9; // edi
	unsigned int v10; // edi
	float xa; // [esp+1Ch] [ebp+4h]
	float xb; // [esp+1Ch] [ebp+4h]

	if (CurrentLevel == LevelIDs_StationSquare)
	{
		v1 = x;
		DisableFog();
		njSetTexture(&texlist_sadxwtr_waves);
		Direct3D_SetZFunc(1u);
		Direct3D_EnableZWrite(0);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		switch (GetTimeOfDay())
		{
		case 0:
			v2 = 21;
			break;
		case 1:
			v2 = 22;
			break;
		case 2:
			v2 = 23;
			break;
		}
		if (CurrentAct != 4 && x->VBuffIndex) v2 = 24;
		SetOceanAlphaModeAndFVF(1);
		njPushMatrix(0);
		v6 = njSin(FrameCounterUnpaused << 6) * x->Offset.y;
		xa = v6;
		z = v6 + v1->Position.z;
		XDist = xa + v1->Position.x;
		njTranslate(0, XDist, v1->Position.y, z);
		njPushMatrix(0);
		xb = xa * 0.5f;
		njTranslate(0, xb, -1.0f, xb);
		njSetTextureNum(v2);
		v9 = 0;
		if (v1->PlaneCount)
		{
			do
			{
				Direct3D_DrawFVF_H(
					OceanGarbageArray[35 * (unsigned __int8)v1->VBuffIndex].points,
					4 * (unsigned __int8)v1->PrimitiveCount);
				njTranslate(0, v1->Offset.x, 0.0, 0.0);
				++v9;
			} while (v9 < (unsigned __int8)v1->PlaneCount);
		}
		njPopMatrix(1u);
		njPushMatrix(0);
		njSetTextureNum(SADXWaveAnimation);
		v10 = 0;
		if (v1->PlaneCount)
		{
			do
			{
				Direct3D_DrawFVF_H(
					OceanGarbageArray[35 * (unsigned __int8)v1->VBuffIndex].points,
					4 * (unsigned __int8)v1->PrimitiveCount);
				njTranslate(0, v1->Offset.x, 0.0, 0.0);
				++v10;
			} while (v10 < (unsigned __int8)v1->PlaneCount);
		}
		njPopMatrix(1u);
		njPopMatrix(1u);
		Direct3D_SetZFunc(1u);
		Direct3D_EnableZWrite(1u);
		ToggleStageFog();
	}
}

void __cdecl MysticRuins_OceanDraw_SADXStyle(OceanData* x)
{
	OceanData* v1; // esi
	float v2; // st7
	float v3; // edx
	float z; // ST1C_4
	float XDist; // ST14_4
	unsigned int v6; // edi
	float xa; // [esp+18h] [ebp+4h]
	float xb; // [esp+18h] [ebp+4h]
	if ((CurrentLevel == LevelIDs_MysticRuins && !CurrentAct) || CurrentLevel == LevelIDs_EggHornet)
	{
		DisableFog();
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njSetTexture(&texlist_sadxwtr_waves);
		Direct3D_SetZFunc(1u);
		Direct3D_EnableZWrite(0);
		SetOceanAlphaModeAndFVF(1);
		njPushMatrix(0);
		v1 = x;
		v2 = njSin(FrameCounterUnpaused << 6) * x->Offset.y;
		v3 = x->Position.y;
		xa = v2;
		z = v2 + v1->Position.z;
		XDist = xa + v1->Position.x;
		njTranslate(0, XDist, v3, z);
		v6 = 0;
		if (v1->PlaneCount)
		{
			xb = xa * 0.5f;
			do
			{
				njPushMatrix(0);
				njTranslate(0, xb, 0.0, xb);
				njSetTextureNum(18);
				Direct3D_DrawFVF_H(
					OceanGarbageArray[35 * (unsigned __int8)v1->VBuffIndex].points,
					4 * (unsigned __int8)v1->PrimitiveCount);
				njPopMatrix(1u);
				njPushMatrix(0);
				njTranslate(0, 0.0, 1.0, 0.0);
				njSetTextureNum(SADXWaveAnimation);
				Direct3D_DrawFVF_H(
					OceanGarbageArray[35 * (unsigned __int8)v1->VBuffIndex].points,
					4 * (unsigned __int8)v1->PrimitiveCount);
				njPopMatrix(1u);
				njTranslate(0, v1->Offset.x, 0.0, 0.0);
				++v6;
			} while (v6 < (unsigned __int8)v1->PlaneCount);
		}
		njPopMatrix(1u);
		j_nullsub_2();
		Direct3D_ResetZFunc();
		Direct3D_EnableZWrite(1u);
		ToggleStageFog();
	}
}

void __cdecl EggCarrier_OceanDraw_SADXStyle(OceanData* arg_0)
{
	Uint32 v1; // ebp
	OceanData* v2; // esi
	float v3; // st7
	float v4; // ecx
	float ZDist; // ST0C_4
	float XDist; // ST04_4
	unsigned int v7; // edi
	int a2; // [esp+18h] [ebp-4h]
	float x; // [esp+20h] [ebp+4h]
	float xz; // [esp+20h] [ebp+4h]

	if ((CurrentLevel == LevelIDs_EggCarrierOutside && CurrentAct <= 2) || CurrentLevel == LevelIDs_Zero || LevelIDs_E101R)
	{
		DisableFog();
		Direct3D_SetNearFarPlanes(SkyboxDrawDistance.Minimum, SkyboxDrawDistance.Maximum);
		DisableFog();
		njSetTexture(&texlist_sadxwtr_waves);
		njPushMatrix(0);
		if (Camera_Data1)
		{
			njTranslate(0, Camera_Data1->Position.x, 0.0, Camera_Data1->Position.z);
		}
		njScale(0, 2.0, 1.0, 2.0);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		v1 = SADXWaveAnimation;
		a2 = 17;
		Direct3D_SetZFunc(3u);
		SetOceanAlphaModeAndFVF(1);
		njPushMatrix(0);
		v2 = arg_0;
		v3 = njSin(FrameCounterUnpaused << 6) * arg_0->Offset.y;
		v4 = arg_0->Position.y;
		x = v3;
		ZDist = v3 + v2->Position.z;
		XDist = x + v2->Position.x;
		njTranslate(0, XDist, v4, ZDist);
		v7 = 0;
		if (v2->PlaneCount)
		{
			xz = x * 0.5f;
			do
			{
				njPushMatrix(0);
				njTranslate(0, xz, 0.0f, xz);
				njSetTextureNum(v1);
				Direct3D_DrawFVF_H(
					OceanGarbageArray[0x23 * (unsigned __int8)v2->VBuffIndex].points,
					4 * (unsigned __int8)v2->PrimitiveCount);
				njPopMatrix(1u);
				njPushMatrix(0);
				njTranslate(0, 0.0f, 2.5f, 0.0f);
				njSetTextureNum(a2);
				Direct3D_DrawFVF_H(
					OceanGarbageArray[0x23 * (unsigned __int8)v2->VBuffIndex].points,
					4 * (unsigned __int8)v2->PrimitiveCount);
				njPopMatrix(1u);
				njTranslate(0, v2->Offset.x, 0.0, 0.0);
				++v7;
			} while (v7 < (unsigned __int8)v2->PlaneCount);
		}
		njPopMatrix(1u);
		j_nullsub_2();
		Direct3D_ResetZFunc();
		njPopMatrix(1u);
		ToggleStageFog();
		Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);
		ToggleStageFog();
	}
}

void __cdecl Past_OceanDraw_SADXStyle(OceanData* x)
{
	Uint32 v1; // edi
	Uint32 v2; // ebx
	OceanData* v3; // esi
	float v4; // st7
	float v5; // ecx
	float z; // ST18_4
	float XDist; // ST10_4
	unsigned int v8; // edi
	unsigned int v9; // edi
	float xa; // [esp+1Ch] [ebp+4h]
	float xb; // [esp+1Ch] [ebp+4h]

	if (CurrentLevel == LevelIDs_Past)
	{
		DisableFog();
		njSetTexture(&texlist_sadxwtr_waves);
		v1 = SADXWaveAnimation;
		if (CurrentAct == 1)
		{
			v2 = 19;
		}
		if (CurrentAct == 2)
		{
			v2 = 20;
		}
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		Direct3D_SetZFunc(1u);
		Direct3D_EnableZWrite(0);
		SetOceanAlphaModeAndFVF(1);
		njPushMatrix(0);
		v3 = x;
		v4 = njSin(FrameCounterUnpaused << 6) * x->Offset.y;
		v5 = x->Position.y;
		xa = v4;
		z = v4 + v3->Position.z;
		XDist = xa + v3->Position.x;
		njTranslate(0, XDist, v5, z);
		njPushMatrix(0);
		xb = xa * 0.5f;
		njTranslate(0, xb, 0.0f, xb);
		njSetTextureNum(v1);
		v8 = 0;
		if (v3->PlaneCount)
		{
			do
			{
				Direct3D_DrawFVF_H(
					OceanGarbageArray[35 * (unsigned __int8)v3->VBuffIndex].points,
					4 * (unsigned __int8)v3->PrimitiveCount);
				njTranslate(0, v3->Offset.x, 0.0, 0.0);
				++v8;
			} while (v8 < (unsigned __int8)v3->PlaneCount);
		}
		njPopMatrix(1u);
		njPushMatrix(0);
		njTranslate(0, 0.0, 1.0, 0.0);
		njSetTextureNum(v2);
		v9 = 0;
		if (v3->PlaneCount)
		{
			do
			{
				Direct3D_DrawFVF_H(
					OceanGarbageArray[35 * (unsigned __int8)v3->VBuffIndex].points,
					4 * (unsigned __int8)v3->PrimitiveCount);
				njTranslate(0, v3->Offset.x, 0.0, 0.0);
				++v9;
			} while (v9 < (unsigned __int8)v3->PlaneCount);
		}
		njPopMatrix(1u);
		njPopMatrix(1u);
		Direct3D_ResetZFunc();
		Direct3D_EnableZWrite(1u);
		ToggleStageFog();
	}
}

int roundfloat(float r)
{
	return int((r > 0.0f) ? (r + 0.5f) : (r - 0.5f));
}

NJS_MODEL_SADX* CloneAttach(NJS_MODEL_SADX* att)
{
	NJS_MODEL_SADX* newatt = new NJS_MODEL_SADX;
	newatt->buffer = nullptr;
	newatt->center = att->center;
	newatt->r = att->r;
	newatt->nbMat = att->nbMat;
	newatt->nbPoint = att->nbPoint;
	newatt->nbMeshset = att->nbMeshset;

	// Vertices and normals
	if (att->points != NULL)
	{
		newatt->points = new NJS_VECTOR[att->nbPoint];
		memcpy(newatt->points, att->points, att->nbPoint * sizeof(NJS_VECTOR));
	}
	else
		newatt->points = NULL;
	if (att->normals != NULL)
	{
		newatt->normals = new NJS_VECTOR[att->nbPoint];
		memcpy(newatt->normals, att->normals, att->nbPoint * sizeof(NJS_VECTOR));
	}
	else
		newatt->normals = NULL;

	// Meshsets
	if (att->nbMeshset > 0)
	{
		newatt->meshsets = new NJS_MESHSET_SADX[att->nbMeshset];
		memcpy(newatt->meshsets, att->meshsets, att->nbMeshset * sizeof(NJS_MESHSET_SADX));
	}
	else
		newatt->meshsets = NULL;

	// Materials
	if (att->nbMat > 0)
	{
		newatt->mats = new NJS_MATERIAL[att->nbMat];
		memcpy(newatt->mats, att->mats, att->nbMat * sizeof(NJS_MATERIAL));
	}
	else
		newatt->mats = NULL;
	return newatt;
}

NJS_OBJECT* CloneObject(NJS_OBJECT* obj)
{
	NJS_OBJECT* newobj = new NJS_OBJECT;
	newobj->ang[0] = obj->ang[0];
	newobj->ang[1] = obj->ang[1];
	newobj->ang[2] = obj->ang[2];
	newobj->evalflags = obj->evalflags;
	newobj->pos[0] = obj->pos[0];
	newobj->pos[1] = obj->pos[1];
	newobj->pos[2] = obj->pos[2];
	newobj->scl[0] = obj->scl[0];
	newobj->scl[1] = obj->scl[1];
	newobj->scl[2] = obj->scl[2];

	// Model
	if (obj->basicdxmodel)
	{
		newobj->basicdxmodel = CloneAttach(obj->basicdxmodel);
	}
	else
		newobj->basicdxmodel = NULL;

	// Child
	if (obj->child)
		newobj->child = CloneObject(obj->child);
	else
		newobj->child = NULL;

	// Sibling
	if (obj->sibling)
		newobj->sibling = CloneObject(obj->sibling);
	else
		newobj->sibling = NULL;

	return newobj;
}

void SADXStyleWater_Init(const char* path, const HelperFunctions& helperFunctions)
{
	helperFunctions.RegisterCommonObjectPVM(SADXWaterPVM1_Waves);
	// Emerald Coast
	if (SADXWater_EmeraldCoast)
	{
		EmeraldCoastWater_Init();
	}
	// Station Square
	if (SADXWater_StationSquare)
	{
		WriteJump(StationSquare_OceanDraw, StationSquare_OceanDraw_SADXStyle);
	}
	// Mystic Ruins
	if (SADXWater_MysticRuins)
	{
		WriteJump(MysticRuins_OceanDraw, MysticRuins_OceanDraw_SADXStyle);
		WriteJump(EggHornet_OceanDraw, MysticRuins_OceanDraw_SADXStyle);
	}
	// Egg Carrier
	if (SADXWater_EggCarrier)
	{
		WriteJump(EggCarrier_OceanDraw, EggCarrier_OceanDraw_SADXStyle);
		WriteJump(E101Mk2_OceanDraw, EggCarrier_OceanDraw_SADXStyle);
		WriteJump(Zero_OceanDraw, EggCarrier_OceanDraw_SADXStyle);
	}
	// Past
	if (SADXWater_Past)
	{
		WriteJump((void*)0x542850, Past_OceanDraw_SADXStyle);
	}
}

void SADXStyleWater_OnFrame()
{
	// Runs only when the game isn't paused
	if ((FramerateSetting < 2 && FrameCounter % 4 == 0) || (FramerateSetting == 2 && FrameCounter % 2 == 0) || FramerateSetting > 2)
	{
		SADXWaveAnimation++;
		if (SADXWaveAnimation > 14) 
			SADXWaveAnimation = 0;
		if (CurrentLevel == LevelIDs_EmeraldCoast)
			EmeraldCoastWater_OnFrame();
	}
}