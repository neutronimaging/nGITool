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



#include "../include/nGIPreprocessing.h"
#include "../include/nGIISSfilter.h"
#include "../include/nGISpotClean.h"
#include <base/KiplException.h>
#include <ModuleException.h>
#include <nGIException.h>

#include <PreprocModuleBase.h>

#include <logging/logger.h>

#include <cstdlib>
#include <map>
#include <list>
#include <string>
#include <sstream>


void * GetModule(const char * application, const char * name)
{
	if (strcmp(application,"ngitool"))
		return NULL;

	if (name!=NULL) {
		std::string sName=name;

		if (sName=="nGISpotClean")
			return new nGISpotClean;

		if (sName=="nGIISSfilter")
			return new nGIISSfilter;
	}

	return NULL;
}

int Destroy(const char * application, void *obj)
{
	if (strcmp(application,"ngitool"))
		return -1;

	kipl::logging::Logger logger("nGIPreprocModules::Destroy");
	std::ostringstream msg;
	std::string name="No name";
	try {
		if (obj!=NULL) {
			PreprocModuleBase *module=reinterpret_cast<PreprocModuleBase *>(obj);
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
		msg<<"Failed to destroy "<<name<<" with Recon exception"<<e.what();

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

int LibVersion()
{
	return -1;
}

int GetModuleList(const char * application, void *listptr)
{
	std::map<std::string, std::map<std::string, std::string> > *modulelist=reinterpret_cast<std::map<std::string, std::map<std::string, std::string> > *>(listptr);

	if (strcmp(application,"ngitool")) {
		modulelist->clear();
		return -1;
	}
	nGISpotClean spot;
	modulelist->operator []("nGISpotClean")=spot.GetParameters();

	nGIISSfilter iss;
	modulelist->operator []("nGIISSfilter")=iss.GetParameters();

	return 0;
}
