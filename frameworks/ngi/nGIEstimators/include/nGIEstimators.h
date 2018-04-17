//
// This file is part of the preprocessing modules recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $Date: 2011-09-22 10:22:46 +0200 (Thu, 22 Sep 2011) $
// $Rev: 1074 $
// $Id: StdPreprocModules.h 1074 2011-09-22 08:22:46Z kaestner $
//

#ifndef _NGIESTIMATORS_H
#define _NGIESTIMATORS_H

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MOREDUMMIES_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MOREDUMMIES_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "ngiestimators_global.h"

#ifdef __cplusplus
extern "C" {
#endif
NGIESTIMATORSSHARED_EXPORT void * GetModule(const char * application, const char * name);
NGIESTIMATORSSHARED_EXPORT int Destroy(const char * application, void *obj);
NGIESTIMATORSSHARED_EXPORT int LibVersion();
NGIESTIMATORSSHARED_EXPORT int GetModuleList(const char * application, void *listptr);
#ifdef __cplusplus
}
#endif

#endif
