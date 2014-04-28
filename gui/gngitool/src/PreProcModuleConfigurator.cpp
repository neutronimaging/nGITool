/*
 * PreProcModuleConfigurator.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: anders
 */

#include "stdafx.h"
#include "../include/PreProcModuleConfigurator.h"
#include <sstream>
#include <ConfigBase.h>
#include <nGIException.h>
#include <ModuleException.h>
#include <base/KiplException.h>
#include <nGIFactory.h>
#include <nGIEngine.h>


PreProcModuleConfigurator::PreProcModuleConfigurator(nGIConfig *config)
{
	ModuleConfigurator::m_Config=dynamic_cast<ConfigBase *>(config);
}

PreProcModuleConfigurator::~PreProcModuleConfigurator() {
}

int PreProcModuleConfigurator::GetImage(std::string sSelectedModule)
{
	nGIEngine *engine=NULL;
	nGIFactory factory;

	nGIConfig * config=dynamic_cast<nGIConfig *>(m_Config);
	std::ostringstream msg;
	try {
		engine=factory.BuildEngine(*config,NULL);
	}
	catch (nGIException &e) {
		msg<<"Failed to build the configuration engine with an nGIException: "<<e.what();
	}
	catch (kipl::base::KiplException &e) {
		msg<<"Failed to build the configuration engine with a KiplException: "<<e.what();
	}
	catch (ModuleException &e){
		msg<<"Failed to build the configuration engine with a ModuleException: "<<e.what();
	}
	catch (std::exception &e) {
		msg<<"Failed to build the configuration engine with a STL exception: "<<e.what();
	}
	catch (...) {
		msg<<"Failed to build the configuration engine with an unhandled exception: ";
	}
	if (!msg.str().empty()) {
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	logger(kipl::logging::Logger::LogMessage,"Engine successfully built");
	size_t *r=config->projections.nROI;
	size_t roi[4]={r[0],r[1],r[2],r[3]};
	m_Image=engine->RunPreprocess(roi,sSelectedModule);

	if (engine!=NULL)
		delete engine;
	return 0;
}
