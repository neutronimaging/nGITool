//<LICENSE>

#ifndef NGIPREPROCMODULES_H
#define NGIPREPROCMODULES_H
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MOREDUMMIES_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MOREDUMMIES_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "ngipreprocessing_global.h"

#ifdef __cplusplus
extern "C" {
#endif
NGIPREPROCESSINGSHARED_EXPORT void * GetModule(const char * application, const char * name);
NGIPREPROCESSINGSHARED_EXPORT int Destroy(const char * application, void *obj);
NGIPREPROCESSINGSHARED_EXPORT int LibVersion();
NGIPREPROCESSINGSHARED_EXPORT int GetModuleList(const char * application, void *listptr);
#ifdef __cplusplus
}
#endif

#endif
