//
// This file is part of the recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-12-06 17:38:48 +0100 (Tue, 06 Dec 2011) $
// $Rev: 1136 $
// $Id: ReconFactory.h 1136 2011-12-06 16:38:48Z kaestner $
//
#ifndef __RECONFACTORY_H
#define __RECONFACTORY_H

#include "nGIFramework_global.h"
#include <logging/logger.h>
#include "nGIEngine.h"
#include "PreprocModuleBase.h"
#include "nGIConfig.h"
#include "InteractionBase.h"
#include <list>
#include <string>

class NGIFRAMEWORKSHARED_EXPORT nGIFactory
{
private:
	kipl::logging::Logger logger;
public:
	nGIFactory(void);
	nGIEngine * BuildEngine(nGIConfig &config, InteractionBase *interactor=NULL);
	virtual ~nGIFactory(void);

private:
};


#endif
