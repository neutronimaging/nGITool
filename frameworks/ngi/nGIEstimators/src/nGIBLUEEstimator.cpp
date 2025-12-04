//<LICENSE>

#include "stdafx.h"
#include "../include/nGIBLUEEstimator.h"
#include <armadillo>
#include <strings/miscstring.h>
#include <filters/filter.h>
#include <filters/filterbase.h>
#include <math/mathfunctions.h>
#include <math/mathconstants.h>
#include <io/io_tiff.h>
#include <strings/filenames.h>
#include <math/compleximage.h>
#include <ParameterHandling.h>
#ifdef _OPENMP
#include <omp.h>
#endif

nGIBLUEEstimator::nGIBLUEEstimator() :
    EstimatorBase("nGIBLUEEstimator",nullptr),
	m_nFilterSize(7)
{

}

nGIBLUEEstimator::~nGIBLUEEstimator()
{

}

int nGIBLUEEstimator::Configure(nGIConfig config, std::map<std::string, std::string> parameters)
{
	mConfig=config;

	PrepareKernel(mConfig.projections.fPeriods,
			mConfig.projections.nPhaseSteps,
			mConfig.projections.bCompletePeriod);


    m_nFilterSize=static_cast<size_t>(GetIntParameter(parameters,"neighborhood"));
	return 0;
}

int nGIBLUEEstimator::Initialize()
{

	return 0;
}

std::map<std::string, std::string> nGIBLUEEstimator::GetParameters()
{
	std::map<std::string, std::string> parameters;

	parameters["neighborhood"]=kipl::strings::value2string(m_nFilterSize);

	return parameters;
}


int nGIBLUEEstimator::ProcessCore(kipl::base::TImage<float,3> proj,
			std::map<std::string, std::string> /*parameters*/,
			kipl::base::TImage<std::complex<float>,2> & H0,
			kipl::base::TImage<std::complex<float>,2> &H1)
{
    logger(kipl::logging::Logger::LogMessage,"Enter process core");
	ComputeVarianceImages(proj);
    logger(kipl::logging::Logger::LogMessage,"Computed variance images");
	kipl::base::TImage<float,3> permuted=PermuteImage(proj);
    logger(kipl::logging::Logger::LogMessage,"Permuted images");
	ComputeHarmonicImage(permuted,H0,H1);

    logger(kipl::logging::Logger::LogMessage,"Leaving process core");
	return 0;
}

void nGIBLUEEstimator::ComputeHarmonicImage(kipl::base::TImage<float,3> img, kipl::base::TImage<std::complex<float>,2> &H0, kipl::base::TImage<std::complex<float>,2> &H1)
{
    std::vector<size_t> dims={img.Size(1),img.Size(2)};

    H0.resize(dims);
    H1.resize(dims);
	ptrdiff_t N=static_cast<ptrdiff_t>(H0.Size());
	float *pImg=img.GetDataPtr();
	float *pVar=m_variance.GetDataPtr();
	complex<float> *pH0=H0.GetDataPtr();
	complex<float> *pH1=H1.GetDataPtr();
	size_t step=img.Size(0);

	ptrdiff_t i=0;
	#pragma omp parallel for
    for (i=0; i<N; i++)
    {
		blue(pImg+i*step,pVar+i*step,step,pH0[i],pH1[i]);
	}
}

void nGIBLUEEstimator::blue(float *data, float *var, int N, complex<float> &H0, complex<float> &H1)
{
    arma::mat HC=m_H;
    arma::mat x(N,1);

    std::copy_n(data,N,x.memptr());

    for (int i=0; i<N; i++)
    {
		for (int j=0; j<3; j++)
            HC.at(i,j) *= var[i];

        x.at(i,0) *= var[i];
	}

    arma::mat m1,m2;

    m2=m_Ht * HC;
    m1=m_Ht * x;

    m2=arma::solve(m2,m1);

    H0=complex<float>(m2.at(0,0),0.0f);
    H1=complex<float>(m2.at(2,0),m2.at(1,0));
}

kipl::base::TImage<float,2> nGIBLUEEstimator::BoxFilter(kipl::base::TImage<float,2> img, size_t dim)
{
    vector<size_t> dimsU={dim,1};
    vector<size_t> dimsV={1,dim};
	size_t N=dim*dim;
    std::vector<float> fKernel(N,1.0f);

	kipl::filters::TFilter<float,2> filterU(fKernel,dimsU);
	kipl::filters::TFilter<float,2> filterV(fKernel,dimsV);

	kipl::base::TImage<float,2> imgU=filterU(img, kipl::filters::FilterBase::EdgeMirror);

    return filterV(imgU, kipl::filters::FilterBase::EdgeMirror);
}

void nGIBLUEEstimator::ComputeVarianceImages(kipl::base::TImage<float,3> img)
{
    kipl::base::TImage<float,2> slice(img.dims());
    m_variance.resize(img.dims());
	// Filter image to compute Variance
	int N=static_cast<int>(img.Size(2));
    for (int i=0; i<N; i++)
    {
		memcpy(slice.GetDataPtr(),img.GetLinePtr(0,i), slice.Size()*sizeof(float));

		kipl::base::TImage<float,2> img2=kipl::math::sqr(slice);

		kipl::base::TImage<float,2> s2=BoxFilter(img2,m_nFilterSize);
		kipl::base::TImage<float,2> m=BoxFilter(slice,m_nFilterSize);

		img2=0.0f;
		float *pRes=img2.GetDataPtr();
		float *pS2=s2.GetDataPtr();
		float *pM=m.GetDataPtr();

		const float M=1.0f/float(m_nFilterSize*m_nFilterSize);
		// const float M1=1.0f/float(m_nFilterSize*m_nFilterSize-1);


		ptrdiff_t j=0;
		ptrdiff_t N2=static_cast<ptrdiff_t>(img2.Size());
		#pragma omp parallel for
        for (j=0; j<N2; j++)
        {
			//pRes[j]=sqrt(M1*(pS2[j]-M*pM[j]*pM[j])); // standard deviation
			pRes[j]=M*(pS2[j]-M*pM[j]*pM[j]);        // Variance
			pRes[j]= pRes[j] !=0.0f ? 1/pRes[j] :0.0f;
		}

		memcpy(m_variance.GetLinePtr(0,i), img2.GetDataPtr(), img2.Size()*sizeof(float));
	}

	// Permute
	m_variance=PermuteImage(m_variance);
}

int nGIBLUEEstimator::PrepareKernel(float fHarmonic, int N, bool /*bCompletePeriod*/)
{
    arma::mat mat(N,3);
    arma::mat mat2(3,N);
	float w= 2.0f*fPi*fHarmonic/static_cast<float>(N - 1);
    for (int i=0; i<N; i++)
    {
        mat2.at(0,i)=mat.at(i,0)=1.0f;
        mat2.at(1,i)=mat.at(i,1)=cos(i*w);
        mat2.at(2,i)=mat.at(i,2)=sin(i*w);
	}

    m_H=mat;
    m_Ht=mat2;
	return 0;
}
