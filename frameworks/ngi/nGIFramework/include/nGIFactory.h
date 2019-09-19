//<LICENSE>
#ifndef NGIFACTORY_H
#define NGIFACTORY_H

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
    nGIEngine * BuildEngine(nGIConfig &config, InteractionBase *interactor=nullptr);
	virtual ~nGIFactory(void);

private:
};


#endif
