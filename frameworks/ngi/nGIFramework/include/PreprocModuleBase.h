//
// This file is part of the Inspector library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-09-22 10:22:46 +0200 (Thu, 22 Sep 2011) $
// $Rev: 1074 $
// $Id: PreprocModuleBase.h 1074 2011-09-22 08:22:46Z kaestner $
//

#ifndef __PREPROCMODULEBASE_H
#define __PREPROCMODULEBASE_H
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
