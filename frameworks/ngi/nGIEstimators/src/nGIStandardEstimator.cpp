//<LICENSE>

#include "stdafx.h"
#include "../include/nGIStandardEstimator.h"
#include <math/mathconstants.h>
#ifdef _OPENMP
#include <omp.h>
#endif

nGIStandardEstimator::nGIStandardEstimator() :
    EstimatorBase("nGIStandardEstimator",nullptr),
    sin_LUT(nullptr),
    cos_LUT(nullptr)
{
}

nGIStandardEstimator::~nGIStandardEstimator() {
    if (sin_LUT!=nullptr)
		delete [] sin_LUT;
    if (cos_LUT!=nullptr)
		delete [] cos_LUT;

}

int nGIStandardEstimator::Configure(nGIConfig config, std::map<std::string, std::string> /*parameters*/)
{
	logger(kipl::logging::Logger::LogVerbose,"module configured");
	mConfig = config;
	 // do something about the parameters.
	Initialize();
	return 0;
}

int nGIStandardEstimator::Initialize()
{
    if (sin_LUT!=nullptr)
		delete [] sin_LUT;
    if (cos_LUT!=nullptr)
		delete [] cos_LUT;

	sin_LUT=new float[mConfig.projections.nPhaseSteps];
	cos_LUT=new float[mConfig.projections.nPhaseSteps];

	return 0;
}

int nGIStandardEstimator::PrepareLUT(int nHarmonic)
{
	std::ostringstream msg;

	msg<<"Preparing LUT with harmonic "<<nHarmonic<<" with "<<mConfig.projections.fPeriods<<" periods";
	logger(kipl::logging::Logger::LogVerbose,msg.str());
	float inc=2.0f*fPi*static_cast<float>(nHarmonic);
	if (mConfig.projections.bCompletePeriod==true)
		inc=inc/static_cast<float>(mConfig.projections.nPhaseSteps-1);
	else
		inc=inc/static_cast<float>(mConfig.projections.nPhaseSteps);

	for (size_t i=0; i<mConfig.projections.nPhaseSteps; i++) {
		sin_LUT[i]=sin(i*inc);
		cos_LUT[i]=cos(i*inc);
	}

	return 0;
}

std::map<std::string, std::string> nGIStandardEstimator::GetParameters()
{
	std::map<std::string, std::string> parameters;

	return parameters;
}

int nGIStandardEstimator::ProcessCore(kipl::base::TImage<float,3> proj,
		std::map<std::string, std::string> /*parameters*/,
		kipl::base::TImage<std::complex<float>,2> & H0,
		kipl::base::TImage<std::complex<float>,2> &H1)
{
	logger(kipl::logging::Logger::LogVerbose,"Process core.");
	kipl::base::TImage<float,3> permuted=PermuteImage(proj);

	PrepareLUT(0);
	H0=ComputeHarmonicImage(permuted);

	PrepareLUT(mConfig.projections.fPeriods);
	H1=ComputeHarmonicImage(permuted);

	return 0;
}

kipl::base::TImage<complex<float> ,2> nGIStandardEstimator::ComputeHarmonicImage(kipl::base::TImage<float,3> imagestack)
{
    auto dims=imagestack.dims();

	std::ostringstream msg;
	msg<<"Stack size="<<imagestack;
	logger(kipl::logging::Logger::LogMessage,msg.str());

	// Todo Transpose is needed
    kipl::base::TImage<complex<float>, 2> result({dims[1],dims[2]});
	ptrdiff_t N=static_cast<ptrdiff_t>(result.Size());
	#pragma omp parallel
	{
		complex<float> * pResult=result.GetDataPtr();
		float *pStack= imagestack.GetDataPtr();
		int i;
		#pragma omp for
		for (i=0; i<N; i++) {
			float real=0.0f;
			float imag=0.0f;
			size_t sidx=i*mConfig.projections.nPhaseSteps;
			for (size_t j=0; j<mConfig.projections.nPhaseSteps; j++, sidx++) {
				real+=cos_LUT[j] * pStack[sidx];
				imag-=sin_LUT[j] * pStack[sidx];
			}
			pResult[i]=complex<float>(real,imag);
		}
	}
	return result;
}
