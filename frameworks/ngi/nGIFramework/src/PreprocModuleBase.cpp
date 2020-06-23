//<LICENSE>

#include "stdafx.h"
#include "../include/nGIConfig.h"
#include "../include/PreprocModuleBase.h"
#include "../include/nGIException.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string.h>

PreprocModuleBase::PreprocModuleBase(std::string name) :
    ProcessModuleBase(name)
{
}

PreprocModuleBase::~PreprocModuleBase(void)
{
	logger(kipl::logging::Logger::LogVerbose,"Destroying module base");
}



int PreprocModuleBase::ExtractSinogram(kipl::base::TImage<float,3> &projections, kipl::base::TImage<float,2> &sinogram, size_t idx)
{
    std::vector<size_t> dims={projections.Size(0), projections.Size(2)};

    sinogram.resize(dims);

	for (size_t i=0; i<dims[1]; i++)
	{
		memcpy(sinogram.GetLinePtr(i),projections.GetLinePtr(idx,i),dims[0]*sizeof(float));
	}

	return 0;
}

int PreprocModuleBase::InsertSinogram(kipl::base::TImage<float,2> &sinogram, kipl::base::TImage<float,3> &projections, size_t idx)
{
    auto dims=sinogram.dims();

	for (size_t i=0; i<dims[1]; i++)
	{
		memcpy(projections.GetLinePtr(idx,i),sinogram.GetLinePtr(i),dims[0]*sizeof(float));
	}

	return 0;
}
