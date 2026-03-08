#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   March 2026
 * ----------------------------------------------------------------------
 */

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#if defined(VNE_SCENE_BUILDING_DLL)
#define VNE_SCENE_API __declspec(dllexport)
#elif defined(VNE_SCENE_DLL)
#define VNE_SCENE_API __declspec(dllimport)
#else
#define VNE_SCENE_API
#endif
#else
#define VNE_SCENE_API
#endif
