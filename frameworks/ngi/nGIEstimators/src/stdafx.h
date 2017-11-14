// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __NGIESTIMATORS_STDAFX_H
#define __NGIESTIMATORS_STDAFX_H

//#include "../include/targetver.h"

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#define DLL_EXPORT_ESTIMATORS __declspec(dllexport)

#else
#ifdef __cplusplus
#define DLL_EXPORT_ESTIMATORS
#else
#define DLL_EXPORT_ESTIMATORS
#endif
#endif


#endif
