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
// $Id: nGIFactory.cpp 1136 2011-12-06 16:38:48Z kaestner $
//
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
	for (it=config.modules.begin(); it!=config.modules.end(); it++) {
		if (it->m_bActive==true) {
			ModuleItem *module=NULL;
			try {
				module=new ModuleItem(it->m_sSharedObject,it->m_sModule);

				module->GetModule()->Configure(config,it->parameters);
				engine->AddPreProcModule(module);
			}
			catch (nGIException &e) {
				throw nGIException(e.what(),__FILE__,__LINE__);
			}
			catch (ModuleException &e) {
				throw ModuleException(e.what(),__FILE__,__LINE__);
			}
			catch (kipl::base::KiplException &e) {
				throw kipl::base::KiplException(e.what(),__FILE__,__LINE__);
			}
		}
	}

	if (config.estimator.m_bActive==true) {
		EstimatorItem *module=NULL;
		try {
			module=new EstimatorItem(config.estimator.m_sSharedObject,config.estimator.m_sModule,interactor);

			module->GetModule()->Configure(config,config.estimator.parameters);
			engine->SetEstimator(module);
		}
		catch (nGIException &e) {
			throw nGIException(e.what(),__FILE__,__LINE__);
		}
		catch (ModuleException &e) {
			throw ModuleException(e.what(),__FILE__,__LINE__);
		}
		catch (kipl::base::KiplException &e) {
			throw kipl::base::KiplException(e.what(),__FILE__,__LINE__);
		}
	}

	return engine;

}

