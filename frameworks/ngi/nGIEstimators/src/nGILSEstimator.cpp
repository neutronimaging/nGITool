//<LICENSE>

#include "stdafx.h"
#include "../include/nGILSEstimator.h"
#include <tnt.h>
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
	EstimatorBase("nGILSEstimator",NULL),
	m_LU(NULL)
{
}

nGILSEstimator::~nGILSEstimator() {
	if (m_LU!=NULL)
		delete m_LU;
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
	size_t dims[2]={img.Size(1),img.Size(2)};

	H0.Resize(dims);
	H1.Resize(dims);
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

	TNT::Array2D<float> x(N,1,data);
	TNT::Array2D<float> m1=TNT::matmult(m_Ht,x);

	TNT::Array2D<float> m2=m_LU->solve(m1);

	H0=complex<float>(m2[0][0],0.0f);
	H1=complex<float>(m2[2][0],m2[1][0]);
}

int nGILSEstimator::PrepareKernel(float fHarmonic, int N, bool bCompletePeriod)
{
	TNT::Array2D<float> mat(N,3);
	TNT::Array2D<float> mat2(3,N);
	float w= 2.0f*fPi*fHarmonic/static_cast<float>(N - 1);
	for (int i=0; i<N; i++) {
		mat2[0][i]=mat[i][0]=1;
		mat2[1][i]=mat[i][1]=cos(i*w);
		mat2[2][i]=mat[i][2]=sin(i*w);
	}

	m_H=mat.copy();
	m_Ht=mat2.copy();

	TNT::Array2D<float> m;

	m=TNT::matmult(m_Ht,m_H);

	if (m_LU!=NULL)
		delete m_LU;

	m_LU=new JAMA::LU<float>(m);

	return 0;
}