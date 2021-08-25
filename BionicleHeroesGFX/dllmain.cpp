// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "pattern/Hooking.Patterns.h"
#include "IniReader.h"
using namespace Memory::VP;


void Init()
{
	CIniReader ini("");

	bool	no_bloom = ini.ReadBoolean("Settings", "DisableBloom", false);
	float	widescreen_val = ini.ReadFloat("Settings", "WidescreenValue", 9.0f / 16.0f);

	// bloom
	auto pattern = hook::pattern("52 6A 34 E8 ? ? ? ? 8B 44 24 38 50 E8 ? ? ? ?");

	if (!pattern.count_hint(1).empty())
	{
		if (no_bloom)
		{
			auto ptr = pattern.get(0).get<int>(0);
			Nop((int)ptr + 13, 5);
		}

	}

	// widescreen value
	pattern = hook::pattern("8B 46 08 C7 40 44 00 00 10 3F");

	if (!pattern.count_hint(1).empty())
	{
		auto ptr = pattern.get(0).get<int>(0);
		Patch<float>((int)ptr + 6, widescreen_val);
	}
}

extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Init();
	}
}