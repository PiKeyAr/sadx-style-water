#pragma once
extern bool SADXWater_EmeraldCoast;
extern bool SADXWater_StationSquare;
extern bool SADXWater_MysticRuins;
extern bool SADXWater_EggCarrier;
extern bool SADXWater_Past;
extern char SADXWaveAnimation;
extern NJS_TEXLIST texlist_sadxwtr_waves;
NJS_OBJECT* CloneObject(NJS_OBJECT* obj);
NJS_MODEL_SADX* CloneAttach(NJS_MODEL_SADX* att);
int roundfloat(float r);
DataPointer(int, FramerateSetting, 0x0389D7DC);