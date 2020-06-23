//<LICENSE>

#include "stdafx.h"
#include "../include/nGIFactory.h"
#include "../include/nGIConfig.h"
#include "../include/PreprocModuleBase.h"
#include "../include/nGIException.h"
#include <base/KiplException.h>
#include <ModuleException.h>

nGIFactory::nGIFactory(void) :
	logger("nGIFactory")
{

}

nGIFactory::~nGIFactory(void)
{

}


nGIEngine * nGIFactory::BuildEngine(nGIConfig &config, InteractionBase *interactor)
{
	nGIEngine * engine=new nGIEngine("nGIEngine",interactor);

	engine->SetConfig(config);
	
	std::list<ModuleConfig>::iterator it;

	// Setting up the preprocessing modules
    for (const auto & module : config.modules)
    {
        if (module.m_bActive==true)
        {
            ModuleItem *moduleObj=nullptr;
            try
            {
                moduleObj=new ModuleItem(module.m_sSharedObject,module.m_sModule);

                moduleObj->GetModule()->Configure(config,module.parameters);
                engine->AddPreProcModule(moduleObj);
			}
            catch (nGIException &e)
            {
				throw nGIException(e.what(),__FILE__,__LINE__);
			}
            catch (ModuleException &e)
            {
				throw ModuleException(e.what(),__FILE__,__LINE__);
			}
            catch (kipl::base::KiplException &e)
            {
				throw kipl::base::KiplException(e.what(),__FILE__,__LINE__);
			}
		}
	}

    if (config.estimator.m_bActive==true)
    {
        EstimatorItem *module=nullptr;
        try
        {
			module=new EstimatorItem(config.estimator.m_sSharedObject,config.estimator.m_sModule,interactor);

			module->GetModule()->Configure(config,config.estimator.parameters);
			engine->SetEstimator(module);
		}
        catch (nGIException &e)
        {
			throw nGIException(e.what(),__FILE__,__LINE__);
		}
        catch (ModuleException &e)
        {
			throw ModuleException(e.what(),__FILE__,__LINE__);
		}
        catch (kipl::base::KiplException &e)
        {
			throw kipl::base::KiplException(e.what(),__FILE__,__LINE__);
		}
	}

	return engine;

}

