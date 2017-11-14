//
// This file is part of the recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-12-11 11:02:54 +0100 (Sun, 11 Dec 2011) $
// $Rev: 1147 $
// $Id: PreprocModuleBase.cpp 1147 2011-12-11 10:02:54Z kaestner $
//

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
	size_t dims[2]={projections.Size(0), projections.Size(2)};

	sinogram.Resize(dims);

	for (size_t i=0; i<dims[1]; i++)
	{
		memcpy(sinogram.GetLinePtr(i),projections.GetLinePtr(idx,i),dims[0]*sizeof(float));
	}

	return 0;
}

int PreprocModuleBase::InsertSinogram(kipl::base::TImage<float,2> &sinogram, kipl::base::TImage<float,3> &projections, size_t idx)
{
	size_t const * const dims=sinogram.Dims();

	for (size_t i=0; i<dims[1]; i++)
	{
		memcpy(projections.GetLinePtr(idx,i),sinogram.GetLinePtr(i),dims[0]*sizeof(float));
	}

	return 0;
}
