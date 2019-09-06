//<LICENSE>

#ifndef PREPROCMODULEBASE_H
#define PREPROCMODULEBASE_H
#include "nGIFramework_global.h"

#include <logging/logger.h>
#include <base/timage.h>
#include <profile/Timer.h>
#include <string>
#include <map>

#include <ProcessModuleBase.h>

#include "nGIConfig.h"

class NGIFRAMEWORKSHARED_EXPORT PreprocModuleBase : public ProcessModuleBase
{
public:
	PreprocModuleBase(std::string name="PreprocModuleBase");
	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters)=0;
	virtual bool SetROI(size_t *roi) {return false;}
	virtual ~PreprocModuleBase(void);

protected:
	int ExtractSinogram(kipl::base::TImage<float,3> &projections, kipl::base::TImage<float,2> &sinogram, size_t idx);
	int InsertSinogram(kipl::base::TImage<float,2> &sinogram, kipl::base::TImage<float,3> &projections, size_t idx);
	virtual int Configure(std::map<std::string, std::string> parameters) {return 0;}
};


#endif
