//<LICENSE>

#include "stdafx.h"
#include "../include/EstimatorBase.h"
#include "../include/nGIException.h"
#include <math/mathconstants.h>
#include <math/compleximage.h>
#include <io/io_tiff.h>
#include <math/median.h>
#include <complex>
#ifdef _OPENMP
#include <omp.h>
#endif

EstimatorBase::EstimatorBase(std::string name, InteractionBase *interactor) :
	logger(name),
    mConfig(""),
	m_sModuleName(name),
	m_bSaveHarmonicImages(false),
	m_Interactor(interactor)
{
	logger(kipl::logging::Logger::LogVerbose,"C'tor EstimatorBase");
    if (m_Interactor!=nullptr) {
		logger(kipl::logging::Logger::LogVerbose,"Got an interactor");
	}
	else {
		logger(kipl::logging::Logger::LogVerbose,"An interactor was not provided");

	}
}

EstimatorBase::~EstimatorBase(void)
{
}

size_t EstimatorBase::Process(kipl::base::TImage<float,3> proj, std::map<std::string, std::string> parameters)
{
	ProcessCore(proj,parameters,m_SampleH0,m_SampleH1);

	return 0;
}

size_t EstimatorBase::ProcessReferences(kipl::base::TImage<float,3> proj, std::map<std::string, std::string> parameters)
{

	ProcessCore(proj,parameters,m_ReferenceH0,m_ReferenceH1);

	return 0;
}

bool EstimatorBase::UpdateStatus(float val, std::string msg)
{
	if (m_Interactor!=NULL) {
		return m_Interactor->SetProgress(val,msg);
	}

	return false;
}

void EstimatorBase::GetProcessedImages(kipl::base::TImage<float,2> & transmission, kipl::base::TImage<float,2> & phase, kipl::base::TImage<float,2> &darkfield)
{
	using namespace std;

	if ((m_SampleH0.Size()==0) || (m_SampleH1.Size()==0) || (m_ReferenceH0.Size()==0) || (m_ReferenceH1.Size()==0))
		throw nGIException("The harmonic images are empty",__FILE__,__LINE__);

    transmission.resize(m_SampleH0.dims());
    phase.resize(m_SampleH0.dims());
    darkfield.resize(m_SampleH0.dims());

	transmission=0.0f;
	phase=0.0f;
	darkfield=0.0f;
    int N=static_cast<int>(m_SampleH0.Size());

    float *pTransmission  = transmission.GetDataPtr();
    float *pPhaseContrast = phase.GetDataPtr();
    float *pDarkField     = darkfield.GetDataPtr();

    std::complex<float> *pSampleDC   = m_SampleH0.GetDataPtr();
    std::complex<float> *pSampleH    = m_SampleH1.GetDataPtr();
	std::complex<float> *pOpenBeamDC = m_ReferenceH0.GetDataPtr();
    std::complex<float> *pOpenBeamH  = m_ReferenceH1.GetDataPtr();

	#pragma omp parallel for
    for (int i=0; i<N; i++)
    {
		pTransmission[i]=abs(pSampleDC[i])/abs(pOpenBeamDC[i]);

	//	pPhaseContrast[i]=WrapDifference(atan2(pSampleH[i].real(),pSampleH[i].imag()),atan2(pOpenBeamH[i].real(),pOpenBeamH[i].imag()));
	//	pPhaseContrast[i]=WrapDifference(atan(pSampleH[i].imag()/pSampleH[i].real()),atan(pOpenBeamH[i].imag()/pOpenBeamH[i].real()));
		pPhaseContrast[i]=WrapDifference(arg(pSampleH[i]),arg(pOpenBeamH[i]));
		pDarkField[i]=abs(pSampleH[i])*abs(pOpenBeamDC[i])/(abs(pSampleDC[i]) * abs(pOpenBeamH[i]));

	}

    if (mConfig.process.bUseAmplLimits)
    {
        for (int i=0; i<N; i++)
        {
			if (mConfig.process.fAmplLimits[1]<pTransmission[i]) pTransmission[i]=mConfig.process.fAmplLimits[1];
			if (pTransmission[i]<mConfig.process.fAmplLimits[0]) pTransmission[i]=mConfig.process.fAmplLimits[0];
		}
	}

    if (mConfig.process.bUseDFILimits)
    {
        for (int i=0; i<N; i++)
        {
			if (mConfig.process.fDFILimits[1]<pDarkField[i]) pDarkField[i]=mConfig.process.fDFILimits[1];
			if (pDarkField[i]<mConfig.process.fDFILimits[0]) pDarkField[i]=mConfig.process.fDFILimits[0];
		}
	}

}

int EstimatorBase::ComputeResidualImage(kipl::base::TImage<float,3> proj)
{
	// todo Implement the residual image processing
	return 0;
}

float EstimatorBase::WrapDifference(float a, float b)
{
	// Wrap phase
	const float minang=-fPi;
	float wa = minang + fmod(a - minang, 2.0f*fPi);
	float wb = minang + fmod(b - minang, 2.0f*fPi);

	float w=wa-wb+fPi;
	float result = w - 2*fPi*floor(w/(2*fPi)) - fPi;

	return result;
}

kipl::base::TImage<float,3> EstimatorBase::PermuteImage(kipl::base::TImage<float,3> & img)
{
    std::vector<size_t> dims={img.Size(2),img.Size(0),img.Size(1)};

	kipl::base::TImage<float,3> permuted(dims);

    float *pImg  = img.GetDataPtr();
    float *pPerm = permuted.GetDataPtr();

	ptrdiff_t zx=dims[0]*dims[1];
	ptrdiff_t nX=img.Size(0);
	ptrdiff_t nY=img.Size(1);
	ptrdiff_t nZ=img.Size(2);

    for (ptrdiff_t z=0,i=0; z<nZ; z++)
    {
        for (ptrdiff_t y=0; y<nY; y++)
        {
            for (ptrdiff_t x=0; x<nX; x++, i++)
            {
				pPerm[z+y*zx+x*dims[0]]=pImg[i];
			}
		}
	}

	return permuted;
}

kipl::base::TImage<float,2> EstimatorBase::ComputeVisibilityMap()
{
    kipl::base::TImage<float,2> vismap(m_ReferenceH0.dims());

    const int N=static_cast<int>(vismap.Size());

	complex<float> *pH=m_ReferenceH1.GetDataPtr();
	complex<float> *pDC=m_ReferenceH0.GetDataPtr();
	float *pMap=vismap.GetDataPtr();

	#pragma omp parallel for
	for (int i=0; i<N; i++)
	{
		pMap[i]=2.0f*abs(pH[i])/abs(pDC[i]);
	}

	return vismap;
}

float EstimatorBase::ComputeVisibility(size_t *roi)
{
    if (roi==NULL)
        return -1.0f;

	std::ostringstream msg;

	if (m_ReferenceH0.Size()==0)
        return -1.0f;

    msg.str(""); msg<<"Computing visibility @ ("<<roi[0]<<", "<<roi[1]<<", "<<roi[2]<<", "<<roi[3]<<")";
	logger(kipl::logging::Logger::LogMessage, msg.str());

	float visibility=0.0f;
    int window=roi[3]-roi[1];
    float *pVis=new float[window];
	memset(pVis,0,sizeof(float)*window);
	float sum=0.0f;

    for (int y=0; y<window; y++) {
        complex<float> *pH=m_ReferenceH1.GetLinePtr(y);
        complex<float> *pDC=m_ReferenceH0.GetLinePtr(y);

        for (int x=roi[0]; x<roi[2]; x++)
		{
            float vis=2.0f*abs(pH[x])/abs(pDC[x]);
			pVis[y]+=vis;
			sum+=vis;
		}
	}

	kipl::math::median(pVis,window,&visibility);
	delete [] pVis;

    return visibility/(roi[2]-roi[0]);
}

void EstimatorBase::SaveHarmonicImages()
{
	if (m_bSaveHarmonicImages==true) {
		std::ostringstream fname;

		fname<<m_sModuleName<<"_RefH0imag.tif";
        kipl::io::WriteTIFF(kipl::complexmath::imag(m_ReferenceH0),fname.str(),kipl::base::Float32);
		fname.str("");
		fname<<m_sModuleName<<"_RefH0real.tif";
        kipl::io::WriteTIFF(kipl::complexmath::real(m_ReferenceH0),fname.str(),kipl::base::Float32);

		fname.str("");
		fname<<m_sModuleName<<"_RefH1imag.tif";
        kipl::io::WriteTIFF(kipl::complexmath::imag(m_ReferenceH1),fname.str(),kipl::base::Float32);
		fname.str("");
		fname<<m_sModuleName<<"_RefH1real.tif";
        kipl::io::WriteTIFF(kipl::complexmath::real(m_ReferenceH1),fname.str(),kipl::base::Float32);

		fname.str("");
		fname<<m_sModuleName<<"_SampleH0imag.tif";
        kipl::io::WriteTIFF(kipl::complexmath::imag(m_SampleH0),fname.str(),kipl::base::Float32);
		fname.str("");
		fname<<m_sModuleName<<"_SampleH0real.tif";
        kipl::io::WriteTIFF(kipl::complexmath::real(m_SampleH0),fname.str(),kipl::base::Float32);
		fname.str("");
		fname<<m_sModuleName<<"_SampleH1imag.tif";
        kipl::io::WriteTIFF(kipl::complexmath::imag(m_SampleH1),fname.str(),kipl::base::Float32);
		fname.str("");
		fname<<m_sModuleName<<"_SampleH1real.tif";
        kipl::io::WriteTIFF(kipl::complexmath::real(m_SampleH1),fname.str(),kipl::base::Float32);
	}
}

