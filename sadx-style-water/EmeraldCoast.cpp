#include <SADXModLoader.h>
#include "Common.h"

NJS_OBJECT* BigDeco1 = nullptr;
NJS_OBJECT* BigDeco2 = nullptr;
NJS_OBJECT* BigDeco3 = nullptr;
DataArray(NJS_TEX, uvSTG01_00CC0530_d, 0x10C0530, 4); // Static ocean default UVs
DataArray(NJS_TEX, uvSTG01_00CBB000_d, 0x10BB000, 1300); // Dynamic ocean default UVs
static bool LilOcean = false;
DataArray(__int16, word_7EC250, 0x7EC250, 16);
DataPointer(OceanData, OceanDataA, 0x03D0B8F0);
DataPointer(OceanData, OceanDataB, 0x03D0B90C);
DataPointer(float, EC1OceanYShift, 0x010C85A8);
DataPointer(float, flt_7E00DC, 0x7E00DC);
DataPointer(float, flt_7DF1B0, 0x7DF1B0);
DataPointer(int, EffectActive, 0x3C5E4B0);
static float float1 = 0.02f;
static float float2 = 66.0f;
static bool InsideSecretArea = false;
NJS_OBJECT* LowPolyOcean = nullptr;
NJS_OBJECT* HighPolyOcean_Static = nullptr;
NJS_OBJECT* HighPolyOcean_Dynamic = nullptr;
NJS_TEX* HighPolyOceanUVs_Dynamic = nullptr;
NJS_TEX* HighPolyOceanUVs_Static = nullptr;
NJS_TEX* LowPolyOceanUVs = nullptr;
static NJS_VECTOR oldpos{ 0,0,0 };


void __cdecl EmeraldCoast_OceanDraw_SADXStyle(OceanData* o)
{
	OceanData* _o; // esi
	char v2; // al
	char v3; // cl
	float f; // st7
	float z; // ST1C_4
	float x; // ST14_4
	unsigned int v7; // edi
	unsigned int v8; // edi
	float _f; // [esp+18h] [ebp+4h]
	float xb; // [esp+18h] [ebp+4h]
	if (CurrentLevel == LevelIDs_EmeraldCoast)
	{
		DisableFog();
		Direct3D_SetZFunc(3u);
		Direct3D_EnableZWrite(0);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njSetTexture(&texlist_sadxwtr_waves);
		SetOceanAlphaModeAndFVF(1);
		njPushMatrix(0);
		_o = o;
		if (GameState != 16)
		{
			v2 = o->OtherIndexIdk--;
			if (!v2)
			{
				v3 = o->TextureIndex + 1;
				o->TextureIndex = v3;
				if (word_7EC250[v3] == -1)
				{
					o->TextureIndex = 0;
				}
				o->OtherIndexIdk = HIBYTE(word_7EC250[o->TextureIndex]);
			}
		}
		f = njSin(FrameCounterUnpaused << 6) * o->Offset.y;
		_f = f;
		z = f + _o->Position.z;
		x = _f + _o->Position.x;
		njTranslate(0, x, _o->Position.y, z);
		njPushMatrix(0);
		njSetTextureNum(_o->VBuffIndex + 15);
		v8 = 0;
		if (_o->PlaneCount)
		{
			do
			{
				Direct3D_DrawFVF_H(
					OceanGarbageArray[0x23 * (unsigned __int8)_o->VBuffIndex].points,
					4 * (unsigned __int8)_o->PrimitiveCount);
				njTranslate(0, _o->Offset.x, 0.0, 0.0);
				++v8;
			} while (v8 < (unsigned __int8)_o->PlaneCount);
		}
		njPopMatrix(1u);
		njPushMatrix(0);
		xb = _f * 0.5f;
		njTranslate(0, xb, 0.2f, xb);
		njSetTextureNum(SADXWaveAnimation);
		v7 = 0;
		if (_o->PlaneCount)
		{
			do
			{
				Direct3D_DrawFVF_H(
					OceanGarbageArray[0x23 * (unsigned __int8)_o->VBuffIndex].points,
					4 * (unsigned __int8)_o->PrimitiveCount);
				njTranslate(0, _o->Offset.x, 0.0, 0.0);
				++v7;
			} while (v7 < (unsigned __int8)_o->PlaneCount);
		}
		njPopMatrix(1u);
		njPopMatrix(1u);
		j_nullsub_2();
		Direct3D_ResetZFunc();
		Direct3D_EnableZWrite(1u);
		ToggleStageFog();
	}
}

void __cdecl Obj_EC23Water_DisplayX(ObjectMaster* a1)
{
	EntityData1* v1; // esi@1
	float x; // ST14_4
	double y; // st7
	double z; // st6
	v1 = a1->Data1;
	// Draw decorations in Big's level
	if (CurrentAct == 2 && BigDeco1 && BigDeco2 && BigDeco3)
	{
		njSetTexture((NJS_TEXLIST*)&texlist_sadxwtr_waves);
		njPushMatrix(0);
		njTranslate(0, 0, 0, 0);
		ds_DrawObjectClip(BigDeco1, 1.0f);
		ds_DrawObjectClip(BigDeco2, 1.0f);
		ds_DrawObjectClip(BigDeco3, 1.0f);
		njPopMatrix(1u);
	}
	// Draw main water
	if (!InsideSecretArea || EV_MainThread_ptr != nullptr)
	{
		DisableFog();
		if (CurrentAct == 2)
		{
			OceanDataA.Position.z = -800.0f;
			OceanDataA.Position.x = 4000.0f;
		}
		DrawModelCallback_Queue((void(__cdecl*)(void*))EmeraldCoast_OceanDraw_SADXStyle, &OceanDataA, -17952.0f, (QueuedModelFlagsB)0); // Main water
	}
	// Draw second pool (Act 2 only because Act 3 looks pretty awful without vertex colors)
	if (CurrentAct == 1)
	{
		x = OceanDataArray[1].Position.x - Camera_Data1->Position.x;
		y = OceanDataArray[1].Position.y - Camera_Data1->Position.y;
		z = OceanDataArray[1].Position.z - Camera_Data1->Position.z;
		if (z * z + y * y + x * x < 9000000.0)
		{
			DrawModelCallback_Queue((void(__cdecl*)(void*))EmeraldCoast_OceanDraw_SADXStyle, &OceanDataArray[1], -17952.0f, (QueuedModelFlagsB)0); // Second pool
		}
	}
}

void __cdecl Obj_EC1Water_DisplayX(ObjectMaster* a1)
{
	auto entity = EntityData1Ptrs[0];
	float v2;
	//int OceanUVShift1;
	EntityData1* v1; // esi@1
	v1 = a1->Data1;
	int unitsize_u_small = 10;
	int unitsize_v_small = 10;
	int u2_add;
	int v2_add;
	int u2_delta;
	int v2_delta;
	DisableFog();
	if (*(signed int*)&v1->CharIndex)
	{
		v2 = njSin(FrameCounterUnpaused << 11) * 1.5f + 0.2f;
		EC1OceanYShift = v2;
	}
	else
	{
		if (EC1OceanYShift > -1.5f)
		{
			EC1OceanYShift = EC1OceanYShift - 0.1f;
		}
	}
	if (entity != nullptr)
	{
		if (entity->Position.x > 1800)
		{
			LilOcean = true;
			WriteData((float**)0x00501824, &flt_7E00DC);
			WriteData((float**)0x00501849, &flt_7E00DC);
			WriteData((float**)0x00501832, &flt_7DF1B0);
			WriteData((float**)0x0050185B, &flt_7DF1B0);
		}
		if (entity->Position.x < 1400) LilOcean = false;
	}
	if (!LilOcean)
	{
		if (v1->CharIndex)
		{
			v2 = njSin(FrameCounterUnpaused << 11) * 3.0f + 2.5f;
			EC1OceanYShift = v2;
			OceanDataA.Position.y = v2;
		}
		else
		{
			if (EC1OceanYShift > -1.5f)
			{
				EC1OceanYShift = EC1OceanYShift - 0.1f;
			}
			OceanDataA.Position.y = EC1OceanYShift;
		}
		DrawModelCallback_Queue((void(__cdecl*)(void*))EmeraldCoast_OceanDraw_SADXStyle, &OceanDataA, -17952.0f, (QueuedModelFlagsB)0);
	}
	else
	{
		if (!MissedFrames)
		{
			njSetTexture(&BEACH_SEA_TEXLIST);
			if (oldpos.x != v1->Position.x)
			{
				u2_add = int(255.0f * (v1->Position.x - oldpos.x) / (float)unitsize_u_small) % 255;
				u2_add = roundfloat(1.5f * u2_add);
				for (int u_step = 0; u_step < 1300; u_step++)
				{
					HighPolyOceanUVs_Dynamic[u_step].u = HighPolyOceanUVs_Dynamic[u_step].u - u2_add;
					u2_delta = HighPolyOceanUVs_Dynamic[u_step].u - uvSTG01_00CBB000_d[u_step].u;
					HighPolyOceanUVs_Dynamic[u_step].u = uvSTG01_00CBB000_d[u_step].u + (u2_delta % 255);
					HighPolyOceanUVs_Static[u_step].u = HighPolyOceanUVs_Dynamic[u_step].u;
				}
			}
			if (oldpos.z != v1->Position.z)
			{
				v2_add = int(255.0f * (v1->Position.z - oldpos.z) / (float)unitsize_v_small) % 255;
				v2_add = roundfloat(0.5f * v2_add);
				for (int v_step = 0; v_step < 1300; v_step++)
				{
					HighPolyOceanUVs_Dynamic[v_step].v = HighPolyOceanUVs_Dynamic[v_step].v - v2_add;
					v2_delta = HighPolyOceanUVs_Dynamic[v_step].v - uvSTG01_00CBB000_d[v_step].v;
					HighPolyOceanUVs_Dynamic[v_step].v = uvSTG01_00CBB000_d[v_step].v + (v2_delta % 255);
					HighPolyOceanUVs_Static[v_step].v = HighPolyOceanUVs_Dynamic[v_step].v;
				}
			}
			oldpos.x = v1->Position.x;
			oldpos.z = v1->Position.z;
			njPushMatrix(0);
			njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z);
			DrawQueueDepthBias = -19952.0f;
			EC1OceanYShift = -1.5f;
			if (EffectActive) late_DrawObjectMesh((NJS_OBJECT*)0x010C03FC, 1);
			else late_DrawObjectClip((NJS_OBJECT*)0x010C03FC, QueuedModelFlagsB_SomeTextureThing, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 2000, EC1OceanYShift, v1->Position.z);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 2000, EC1OceanYShift, v1->Position.z + 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 2000, EC1OceanYShift, v1->Position.z - 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z - 2000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z - 2000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z - 2000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 2000, EC1OceanYShift, v1->Position.z);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 2000, EC1OceanYShift, v1->Position.z + 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 2000, EC1OceanYShift, v1->Position.z - 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z + 2000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z + 2000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z + 2000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z + 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z - 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z + 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z - 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z + 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z - 1000);
			late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
			njPopMatrix(1u);
			DrawQueueDepthBias = 0;
		}
	}
}

void EmeraldCoastWater_Init()
{
	HighPolyOcean_Static = CloneObject((NJS_OBJECT*)0x010C03FC);
	HighPolyOcean_Dynamic = (NJS_OBJECT*)0x010C03FC;
	HighPolyOceanUVs_Dynamic = HighPolyOcean_Dynamic->basicdxmodel->meshsets[0].vertuv;
	HighPolyOceanUVs_Static = HighPolyOcean_Static->basicdxmodel->meshsets[0].vertuv;
	HighPolyOcean_Dynamic->basicdxmodel->mats[0].diffuse.argb.a = 0x99; // Match dynamic ocean alpha with normal ocean
	HighPolyOcean_Static->basicdxmodel->mats[0].diffuse.argb.a = 0x99; // Match dynamic ocean alpha with normal ocean
	HighPolyOcean_Dynamic->basicdxmodel->mats[0].attrflags &= ~NJD_DA_INV_SRC;
	HighPolyOcean_Dynamic->basicdxmodel->mats[0].attrflags |= NJD_DA_ONE;
	HighPolyOcean_Dynamic->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF;
	HighPolyOcean_Static->basicdxmodel->mats[0].attrflags &= ~NJD_DA_INV_SRC;
	HighPolyOcean_Static->basicdxmodel->mats[0].attrflags |= NJD_DA_ONE;
	HighPolyOcean_Static->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF;
	WriteJump((void*)0x00501130, Obj_EC1Water_DisplayX); // Act 1
	WriteJump((void*)0x004F76C0, Obj_EC23Water_DisplayX); // Act 2
	WriteJump((void*)0x004F7760, Obj_EC23Water_DisplayX); // Act 3
		// Different UVs on the dynamic ocean model for SADX water
	for (int rq = 0; rq < 1300; rq++)
	{
		uvSTG01_00CBB000_d[rq].u = (Sint16)round(0.5f * uvSTG01_00CBB000_d[rq].u);
		uvSTG01_00CBB000_d[rq].v = (Sint16)round(0.5f * uvSTG01_00CBB000_d[rq].v);
	}
	//BigDeco1 = nullptr;// LoadModel("system\\data\\STG01\\Models\\DX\\00ACA3EC.sa1mdl");
	//BigDeco2 = LoadModel("system\\data\\STG01\\Models\\DX\\00AC97B4.sa1mdl");
	//BigDeco3 = LoadModel("system\\data\\STG01\\Models\\DX\\00ACA028.sa1mdl");
	WriteJump(EmeraldCoast_OceanDraw, EmeraldCoast_OceanDraw_SADXStyle);
}

void EmeraldCoastWater_OnFrame()
{
	if (HighPolyOcean_Dynamic != nullptr)
		HighPolyOcean_Dynamic->basicdxmodel->mats[0].attr_texId = SADXWaveAnimation;
	if (HighPolyOcean_Static != nullptr)
		HighPolyOcean_Static->basicdxmodel->mats[0].attr_texId = SADXWaveAnimation;
}