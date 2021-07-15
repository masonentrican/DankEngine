#pragma once

// Precompiled header to include the standard libs
// used throught the engine without needing to recompile
// them everything the files change

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <algorithm>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "DankEngine/Log.h"

#ifdef DANK_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

// ImGuiRenderer
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif