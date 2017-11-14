//
// This file is part of the preprocessing modules recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $Date: 2012-02-18 17:45:40 +0100 (Sat, 18 Feb 2012) $
// $Rev: 1218 $
// $Id: StdPreprocModules.cpp 1218 2012-02-18 16:45:40Z kaestner $
//

#include "../include/ngiestimators_global.h"

#include "../include/nGIEstimators.h"
#include "../include/nGIStandardEstimator.h"
#include "../include/nGIBLUEEstimator.h"
#include "../include/nGILSEstimator.h"
#include "../include/nGIPenalizedMLEstimator.h"
#include <base/KiplException.h>
#include <ModuleException.h>
#include <nGIException.h>

#include <EstimatorBase.h>

#include <logging/logger.h>

#include <cstdlib>
#include <map>
#include <list>
#include <string>
#include <sstream>


NGIESTIMATORSSHARED_EXPORT void * GetModule(const char * application, const char * name)
{
	if (strcmp(application,"ngitool"))
		return NULL;

	if (name!=NULL) {
		std::string sName=name;

		if (sName=="standardestimator")
			return new nGIStandardEstimator;
		if (sName=="blueestimator")
			return new nGIBLUEEstimator;
		if (sName=="lsestimator")
			return new nGILSEstimator;
		if (sName=="penalizedmlestimator")
			return new nGIPenalizedMLEstimator;
	}

	return NULL;
}

NGIESTIMATORSSHARED_EXPORT int Destroy(const char * application, void *obj)
{
	if (strcmp(application,"ngitool"))
		return -1;

	kipl::logging::Logger logger("nGIEstimators::Destroy");
	std::ostringstream msg;
	std::string name="No name";
	try {
		if (obj!=NULL) {
			EstimatorBase *module=reinterpret_cast<EstimatorBase *>(obj);
			name=module->ModuleName();
			msg<<"Destroying "<<name;
			logger(kipl::logging::Logger::LogVerbose,msg.str());
			msg.str("");
			delete module;
		}
	}
	catch (std::exception & e) {
		msg<<"Failed to destroy "<<name<<" with STL exception"<<e.what();

		logger(kipl::logging::Logger::LogError,msg.str());
		return -1;
	}
	catch (ModuleException & e) {
		msg<<"Failed to destroy "<<name<<" with Module exception"<<e.what();

		logger(kipl::logging::Logger::LogError,msg.str());
		return -1;
	}
	catch (nGIException & e) {
		msg<<"Failed to destroy "<<name<<" with nGI exception"<<e.what();

		logger(kipl::logging::Logger::LogError,msg.str());
		return -1;
	}
	catch (kipl::base::KiplException & e) {
		msg<<"Failed to destroy "<<name<<" with KIPL exception"<<e.what();

		logger(kipl::logging::Logger::LogError,msg.str());
		return -1;
	}

	return 0;
}

NGIESTIMATORSSHARED_EXPORT int LibVersion()
{
	return -1;
}

NGIESTIMATORSSHARED_EXPORT int GetModuleList(const char * application, void *listptr)
{
	std::map<std::string, std::map<std::string, std::string> > *modulelist=reinterpret_cast<std::map<std::string, std::map<std::string, std::string> > *>(listptr);

	if (strcmp(application,"ngitool")) {
		modulelist->clear();
		return -1;
	}

	nGIStandardEstimator stdest;
	modulelist->operator []("standardestimator")=stdest.GetParameters();
	nGIBLUEEstimator blue;
	modulelist->operator []("blueestimator")=blue.GetParameters();

	nGILSEstimator ls;
	modulelist->operator []("lsestimator")=ls.GetParameters();

	nGIPenalizedMLEstimator pml;
	modulelist->operator []("penalizedmlestimator")=pml.GetParameters();

	return 0;
}
