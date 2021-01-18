//<LICENSE>

#include "stdafx.h"
#include "../include/nGILSEstimator.h"
#include <armadillo>
#include <strings/miscstring.h>
#include <filters/filter.h>
#include <filters/filterbase.h>
#include <math/mathfunctions.h>
#include <math/mathconstants.h>
#include <io/io_tiff.h>
#include <strings/filenames.h>
#include <math/compleximage.h>
#ifdef _OPENMP
#include <omp.h>
#endif

nGILSEstimator::nGILSEstimator() :
    EstimatorBase("nGILSEstimator",nullptr)
{
}

nGILSEstimator::~nGILSEstimator() {
}

int nGILSEstimator::Configure(nGIConfig config, std::map<std::string, std::string> parameters)
{
	mConfig=config;

	PrepareKernel(mConfig.projections.fPeriods,
			mConfig.projections.nPhaseSteps,
			mConfig.projections.bCompletePeriod);

	return 0;
}

int nGILSEstimator::Initialize()
{

	return 0;
}

std::map<std::string, std::string> nGILSEstimator::GetParameters()
{
	std::map<std::string, std::string> parameters;

	return parameters;
}


int nGILSEstimator::ProcessCore(kipl::base::TImage<float,3> proj,
			std::map<std::string, std::string> parameters,
			kipl::base::TImage<std::complex<float>,2> & H0,
			kipl::base::TImage<std::complex<float>,2> &H1)
{
	kipl::base::TImage<float,3> permuted=PermuteImage(proj);

	ComputeHarmonicImage(permuted,H0,H1);

	return 0;
}

void nGILSEstimator::ComputeHarmonicImage(kipl::base::TImage<float,3> img, kipl::base::TImage<std::complex<float>,2> &H0, kipl::base::TImage<std::complex<float>,2> &H1)
{
    std::vector<size_t> dims={img.Size(1),img.Size(2)};

    H0.resize(dims);
    H1.resize(dims);
	ptrdiff_t N=static_cast<ptrdiff_t>(H0.Size());
	float *pImg=img.GetDataPtr();

	complex<float> *pH0=H0.GetDataPtr();
	complex<float> *pH1=H1.GetDataPtr();
	size_t step=img.Size(0);

	#pragma omp parallel for
	for (ptrdiff_t i=0; i<N; i++) {
		leastsquare(pImg+i*step,step,pH0[i],pH1[i]);
	}

}

void nGILSEstimator::leastsquare(float *data, int N, complex<float> &H0, complex<float> &H1)
{

    arma::mat x(N,1);
    std::copy_n(data,N,x.memptr());
    arma::mat m1=m_Ht*x;

    arma::vec m2=arma::solve(m_H,m1);

    H0=complex<float>(m2.at(0),0.0f);
    H1=complex<float>(m2.at(2),m2.at(1));
}

int nGILSEstimator::PrepareKernel(float fHarmonic, int N, bool bCompletePeriod)
{
    arma::mat mat(N,3);
    arma::mat mat2(3,N);
	float w= 2.0f*fPi*fHarmonic/static_cast<float>(N - 1);
	for (int i=0; i<N; i++) {
        mat2.at(0,i)=mat.at(i,0)=1;
        mat2.at(1,i)=mat.at(i,1)=cos(i*w);
        mat2.at(2,i)=mat.at(i,2)=sin(i*w);
	}

    m_H=mat;
    m_Ht=mat2;

    arma::mat m=m_Ht*m_H;

	return 0;
}
