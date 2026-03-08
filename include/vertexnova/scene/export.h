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
/* Building the vnescene shared library (CMake sets VNE_SCENE_BUILDING_DLL). */
#if defined(VNE_SCENE_BUILDING_DLL)
#define VNE_SCENE_API __declspec(dllexport)
/* Consuming the vnescene shared library (CMake sets VNE_SCENE_DLL on dependents). */
#elif defined(VNE_SCENE_DLL)
#define VNE_SCENE_API __declspec(dllimport)
/* Static build or unknown: no export/import. */
#else
#define VNE_SCENE_API
#endif
#else
/* Non-Windows: no dllexport/dllimport. */
#define VNE_SCENE_API
#endif
