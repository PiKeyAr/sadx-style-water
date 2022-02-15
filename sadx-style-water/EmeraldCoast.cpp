#include <SADXModLoader.h>

extern int SADXStyleWaterAnimationFrame;
extern NJS_TEXLIST texlist_sadxwater;
extern NJS_TEXLIST texlist_beachsea;

NJS_OBJECT* BigDeco1 = nullptr;
NJS_OBJECT* BigDeco2 = nullptr;
NJS_OBJECT* BigDeco3 = nullptr;
NJS_OBJECT* HighPolyOcean_Static = nullptr;

DataPointer(float, EC1OceanYShift, 0x010C85A8);
DataPointer(int, EffectActive, 0x3C5E4B0);

DataArray(NJS_TEX, DynamicUVsDefault, 0x10BB000, 1300); // Dynamic ocean default UVs
NJS_TEX DynamicUVsOriginal[1300];
NJS_TEX DynamicUVsUpdated[1300];
NJS_TEXLIST texlists_beach_waves[15];

static float OceanModelUVShiftX = 45.5f;
static float OceanModelUVShiftZ = 44.5f;
static bool SmallOceanActivated = false;
static NJS_VECTOR UVShiftOldPosition = { 0,0,0 };

bool DreamcastEmeraldCoast = false;

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

void __cdecl EmeraldCoast_OceanDraw_SADXStyle(stcWaterSurface* pu32SaltStat)
{
	___njFogDisable();
	njSetZCompare(3u);
	njSetZUpdateMode(0);
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
	njSetTexture(&texlist_sadxwater);
	njDrawTexture3DExStart(1);
	njPushMatrix(0);
	if (ssGameMode != 16)
	{
		if (pu32SaltStat->s8Frame0)
		{
			pu32SaltStat->s8AnimNo0++;
			if (cosSaltWaterAnimation[pu32SaltStat->s8Frame0].s8TexNo == 255)
				pu32SaltStat->s8AnimNo0 = 0;
			pu32SaltStat->s8Frame0 = cosSaltWaterAnimation[pu32SaltStat->s8AnimNo0].s8Frame;
		}
	}
	float f = njSin(gu32LocalCnt << 6) * pu32SaltStat->f32TransScale;
	njTranslate(0, f + pu32SaltStat->f32x0, pu32SaltStat->f32y0, f + pu32SaltStat->f32z0);
	njPushMatrix(0);
	njSetTextureNum(pu32SaltStat->u8StripArrayNo + 15); // Results in 15 (ocean) and 16 (pool)
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
	njTranslate(0, f * 0.5f, 0.2f, f * 0.5f);
	njSetTextureNum(SADXStyleWaterAnimationFrame);
	if (pu32SaltStat->u8WrapX)
	{
		for (int v7 = 0; v7 < pu32SaltStat->u8WrapX; v7++)
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

void __cdecl DrawEmeraldCoastOceanAct23(task* a1)
{
	bool InsideCave = (ssActNumber == 2 && camera_twp != nullptr && camera_twp->pos.z > 2000);
	// Draw decorations in Big's level
	if (DreamcastEmeraldCoast && ssActNumber == 2)
	{
		njSetTexture((NJS_TEXLIST*)&texlist_sadxwater);
		njPushMatrix(0);
		njTranslate(0, 0, 0, 0);
		ds_DrawObjectClip(BigDeco1, 1.0f);
		ds_DrawObjectClip(BigDeco2, 1.0f);
		ds_DrawObjectClip(BigDeco3, 1.0f);
		njPopMatrix(1u);
	}
	// Draw main water
	if (!InsideCave || EV_MainThread_ptr != nullptr)
	{
		late_SetFunc((void(__cdecl*)(void*))EmeraldCoast_OceanDraw_SADXStyle, &gasPoolStat[0], -17952.0f, 0);
	}
	// Draw second pool
	if (ssActNumber == 1 || (ssActNumber == 2 && !DreamcastEmeraldCoast))
	{
		float x = gasPoolStat[1].f32x0 - camera_twp->pos.x;
		float y = gasPoolStat[1].f32y0 - camera_twp->pos.y;
		float z = gasPoolStat[1].f32z0 - camera_twp->pos.z;
		if (z * z + y * y + x * x < 9000000.0f)
			late_SetFunc((void(__cdecl*)(void*))EmeraldCoast_OceanDraw_SADXStyle, &gasPoolStat[1], -17952.0f, 0);
	}
}

void __cdecl DrawEmeraldCoastOceanAct1(task* a1)
{
	auto entity = playertwp[0];
	taskwk* work = a1->twp;
	int unitsize_u_small = 10;
	int unitsize_v_small = 10;
	int u2_add;
	int v2_add;
	int u2_delta;
	int v2_delta;
	if (work->counter.l != 0)
	{
		float distort = njSin(gu32LocalCnt << 11);
		EC1OceanYShift = (distort * 3.0f) + 2.5f;
	}
	else if (EC1OceanYShift > -1.5f)
		EC1OceanYShift -= 0.1f;
	gasPoolStat[0].f32y0 = EC1OceanYShift;
	___njFogDisable();
	if (entity != nullptr)
	{
		if (entity->pos.x > 1800)
			SmallOceanActivated = true;
		else if (entity->pos.x < 1400) 
			SmallOceanActivated = false;
	}
	// Draw regular DX ocean
	if (!SmallOceanActivated)
		late_SetFunc((void(__cdecl*)(void*))EmeraldCoast_OceanDraw_SADXStyle, &gasPoolStat[0], -17952.0f, 0);
	// Otherwise draw the high-poly model
	else
	{
		if (!loop_count)
		{
			if (UVShiftOldPosition.x != work->pos.x)
			{
				u2_add = int(255.0f * (work->pos.x - UVShiftOldPosition.x) / (float)unitsize_u_small) % 255;
				for (int u_step = 0; u_step < 1300; u_step++)
				{
					DynamicUVsUpdated[u_step].u = DynamicUVsUpdated[u_step].u - u2_add;
					u2_delta = DynamicUVsUpdated[u_step].u - DynamicUVsOriginal[u_step].u;
					DynamicUVsUpdated[u_step].u = DynamicUVsOriginal[u_step].u + (u2_delta % 255);
				}
			}
			if (UVShiftOldPosition.z != work->pos.z)
			{
				v2_add = int(255.0f * (work->pos.z - UVShiftOldPosition.z) / (float)unitsize_v_small) % 255;
				for (int v_step = 0; v_step < 1300; v_step++)
				{
					DynamicUVsUpdated[v_step].v = DynamicUVsUpdated[v_step].v - v2_add;
					v2_delta = DynamicUVsUpdated[v_step].v - DynamicUVsOriginal[v_step].v;
					DynamicUVsUpdated[v_step].v = DynamicUVsDefault[v_step].v + (v2_delta % 255);
				}
			}
			UVShiftOldPosition.x = work->pos.x;
			UVShiftOldPosition.z = work->pos.z;
			float f = njSin(gu32LocalCnt << 6) * 40;
			LATE flag = EffectActive != 0 ? LATE_WZ : LATE_LIG;

			njPushMatrix(0);
			njTranslate(0, work->pos.x + f, EC1OceanYShift - 2.0f, work->pos.z + f);
			njSetTexture(&texlist_beachsea);
			late_z_ofs___ = -19952.0f;
			late_DrawObjectMesh((NJS_OBJECT*)0x010C03FC, flag);

			njTranslate(0, -f * 0.5f, 2.0f, -f * 0.5f);
			late_z_ofs___ = -18952.0f;
			njSetTexture(&texlists_beach_waves[SADXStyleWaterAnimationFrame]);
			late_DrawObjectMesh((NJS_OBJECT*)0x010C03FC, flag);
			njPopMatrix(1u);

			for (int x = -2; x < 3; x++)
			{
				for (int z = -2; z < 3; z++)
				{
					if (x == 0 && z == 0)
						continue;
					njPushMatrix(0);
					njSetTexture(&texlist_beachsea);
					njTranslate(0, work->pos.x + f + 1000 * x, EC1OceanYShift - 2.0f, work->pos.z + 1000.0f * z + f);
					late_z_ofs___ = -19952.0f;
					late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);

					njTranslate(0, -f * 0.5f, 2.0f, -f * 0.5f);
					njSetTexture(&texlists_beach_waves[SADXStyleWaterAnimationFrame]);
					late_z_ofs___ = -18952.0f;
					late_DrawObjectClip(HighPolyOcean_Static, QueuedModelFlagsB_3, 1.0f);
					njPopMatrix(1u);
				}
			}
			late_z_ofs___ = 0.0f;
		}
	}
}

void EmeraldCoastWater_Init()
{
	WriteData((float**)0x00501832, &OceanModelUVShiftX);
	WriteData((float**)0x0050185B, &OceanModelUVShiftZ);
	for (int i = 0; i < 15; i++)
	{
		texlists_beach_waves[i].nbTexture = 1;
		texlists_beach_waves[i].textures = &texlist_sadxwater.textures[i];
	}
	((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF;
	((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].attrflags = 0x86302400;
	// Different UVs on the dynamic ocean model for SADX water
	for (int rq = 0; rq < 1300; rq++)
	{
		DynamicUVsDefault[rq].u = (Sint16)round(0.5f * DynamicUVsDefault[rq].u);
		DynamicUVsDefault[rq].v = (Sint16)round(0.5f * DynamicUVsDefault[rq].v);
	}
	HighPolyOcean_Static = CloneObject((NJS_OBJECT*)0x010C03FC); // This model is used to surround the distorting model
	for (int i = 0; i < 1300; i++)
	{
		DynamicUVsOriginal[i].u = DynamicUVsDefault[i].u;
		DynamicUVsOriginal[i].v = DynamicUVsDefault[i].v;
	}
	HighPolyOcean_Static->basicdxmodel->meshsets[0].vertuv = DynamicUVsUpdated;
	((NJS_OBJECT*)0x010C03FC)->basicdxmodel->meshsets[0].vertuv = DynamicUVsUpdated;
	WriteData((float*)0x004F8D2F, -2153.0f); // Remove gap in Act 2 small pool
	WriteData((float*)0x004F8C8F, -109.0f); // Remove gap in Act 3 small pool
	WriteData((float*)0x004F8C38, 4000.0f); // Offset main ocean X in Act 3... I forgot why
	WriteData((float*)0x004F8C42, -800.0f); // Offset main ocean Z in Act 3... I forgot why
	// Write water rendering functions
	WriteJump((void*)0x00501130, DrawEmeraldCoastOceanAct1); // Act 1
	WriteJump((void*)0x004F76C0, DrawEmeraldCoastOceanAct23); // Act 2
	WriteJump((void*)0x004F7760, DrawEmeraldCoastOceanAct23); // Act 3
	WriteJump(EmeraldCoast_OceanDraw, EmeraldCoast_OceanDraw_SADXStyle);
	// Act 3 decorations for Dreamcast Emerald Coast + SADX water
	BigDeco1 = CloneObject((NJS_OBJECT*)0x00ECA3EC);
	BigDeco2 = CloneObject((NJS_OBJECT*)0x00EC97B4);
	BigDeco3 = CloneObject((NJS_OBJECT*)0x00ECA028);
	BigDeco1->basicdxmodel->mats[0].attr_texId =
		BigDeco2->basicdxmodel->mats[0].attr_texId =
		BigDeco3->basicdxmodel->mats[0].attr_texId = 26;
}

void EmeraldCoastWater_OnFrame()
{
	DreamcastEmeraldCoast = objLandTable[10] != (_OBJ_LANDTABLE*)0x10386B4;
	switch (ssGameMode)
	{
	case 3:
	case 4:
	case 7:
	case 21:
		for (int r2 = 0; r2 < 1300; r2++)
		{
			DynamicUVsUpdated[r2].u = DynamicUVsOriginal[r2].u;
			DynamicUVsUpdated[r2].v = DynamicUVsOriginal[r2].v;
		}
		break;
	}
}