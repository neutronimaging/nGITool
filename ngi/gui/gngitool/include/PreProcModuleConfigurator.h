/*
 * PreProcModuleConfigurator.h
 *
 *  Created on: Jan 21, 2012
 *      Author: anders
 */

#ifndef PREPROCMODULECONFIGURATOR_H_
#define PREPROCMODULECONFIGURATOR_H_

#include <ModuleConfigurator.h>
#include <nGIConfig.h>

class PreProcModuleConfigurator: public ModuleConfigurator {
public:
	PreProcModuleConfigurator(nGIConfig *config);
	virtual ~PreProcModuleConfigurator();
protected:
	virtual int GetImage(std::string sSelectedModule);
};

#endif /* PREPROCMODULECONFIGURATOR_H_ */
