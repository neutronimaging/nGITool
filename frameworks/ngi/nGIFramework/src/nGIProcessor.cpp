//<LICENSE>

#include "stdafx.h"
#include "../include/nGIProcessor.h"
#include "../include/nGIException.h"

// kipl includes
#include <strings/filenames.h>
#include <strings/miscstring.h>
#include <io/io_fits.h>
#include <io/io_tiff.h>
#include <math/mathconstants.h>
#include <math/compleximage.h>

// Recon includes

// Standard includes
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <cstring>

DpcProcessor::DpcProcessor(size_t lut_size) :
    mConfig(""),
    logger("nGI Processor"),
    nLUTsize(lut_size),
    sin_LUT(new float[nLUTsize]),
    cos_LUT(new float[nLUTsize]),
    fReferenceDose(1.0f)
{
	memset(sin_LUT,0,sizeof(float)*nLUTsize);
	memset(cos_LUT,0,sizeof(float)*nLUTsize);

	extmap[".fit"]=0;
	extmap[".fits"]=0;
	extmap[".fts"]=0;
	extmap[".tif"]=1;
	extmap[".tiff"]=1;
}

DpcProcessor::~DpcProcessor() {
	delete [] sin_LUT;
	delete [] cos_LUT;
}

int DpcProcessor::Initialize(nGIConfig config)
{
	kipl::base::TImage<float,3> stack;

	mConfig=config;

	LoadDark();

    if (mConfig.projections.bUseNorm==true)
    {
        std::string path;
        std::string mask;
        std::vector<std::string> ext;
        kipl::strings::filenames::StripFileName(mConfig.projections.sReferenceMask,path,mask,ext);

        fReferenceDose=reader.GetProjectionDose(path,
                mask,
				mConfig.projections.nRefFirstIndex,
				mConfig.projections.nNormROI);
    }
	else
		fReferenceDose=1.0f;
	
	// Load and process the open beam
    //kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sReferencePath,true);
    std::string filemask=mConfig.projections.sReferenceMask;
	stack=LoadImageStack(filemask,mConfig.projections.nRefFirstIndex,mConfig.projections.nRefFileStride);

	PrepareLUT(0);
	openbeamDC=ComputeHarmonicImage(stack);

	PrepareLUT(1);
	openbeamH=ComputeHarmonicImage(stack);

	return 0;
}

int DpcProcessor::Process(int frame)
{
	kipl::base::TImage<float,3> stack;

//	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sPath,true);
    std::string filemask=mConfig.projections.sProjectionMask;

    int index = static_cast<int>(mConfig.projections.nFirstIndex);
	index += mConfig.projections.nFileStride==1 ? frame*mConfig.projections.nPhaseSteps  : frame;

	stack=LoadImageStack(filemask,index, mConfig.projections.nFileStride);

	PrepareLUT(0);
	sampleDC=ComputeHarmonicImage(stack);

	PrepareLUT(1);
	sampleH=ComputeHarmonicImage(stack);

	ComputeResultImages();
	return 0;
}

void DpcProcessor::LoadDark()
{
	std::string filename;
	std::string ext;
	kipl::base::TImage<float,2> img;
    std::vector<size_t> roi={};
	if (mConfig.projections.bUseROI==true)
		roi=mConfig.projections.nROI;

    //kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sDarkPath,true);
    std::string filemask=mConfig.projections.sDarkMask;
	ostringstream msg;

    try
    {
        for (size_t i=0; i<mConfig.projections.nDarkCnt; i++)
        {
	//		img=reader.Read(mConfig.projections.sDarkPath,mConfig.projections.sDarkMask,mConfig.projections.nDarkFirstIndex+i,roi);
			
			if (i==0)
				dark=img;
			else
				dark+=img;
		}
	}
    catch (kipl::base::KiplException &e)
    {
		msg<<"A kipl exception was thrown during dark file reading\n"<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
    catch (std::exception &e)
    {
		msg<<"An STL exception was thrown during dark file reading\n"<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
    catch (...)
    {
		msg<<"An unknown exception was thrown during dark file reading\n";
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	dark/=static_cast<float>(mConfig.projections.nDarkCnt);
}

void DpcProcessor::ComputeOscillationPlot(kipl::base::TImage<float,3> stack)
{
    auto dims=stack.dims();

	size_t start_x = (dims[0]>>1)-5;
	size_t end_x   = (dims[0]>>1)+5;
	size_t start_y = (dims[1]>>1)-5;
	size_t end_y   = (dims[1]>>1)+5;

    float *pLine=nullptr;
    for (size_t z=0; z<dims[2]; z++)
    {
		oscillation[z]=0.0f;
        for (size_t y=start_y; y<end_y; y++)
        {
			pLine=stack.GetLinePtr(y,z);
            for (size_t x=start_x; y<end_x; x++)
            {
				oscillation[z]+=pLine[x];
			}
		}
		oscillation[z]*=0.01f;
		std::clog<<oscillation[z]<<std::endl;
	}
}

void DpcProcessor::PrepareLUT(size_t nHarmonic)
{
	std::ostringstream msg;

	msg<<"Preparing LUT with harmonic "<<nHarmonic<<" with "<<mConfig.projections.fPeriods<<" periods";
	logger(kipl::logging::Logger::LogVerbose,msg.str());
	float inc=2.0f*fPi*static_cast<float>(nHarmonic*mConfig.projections.fPeriods);
	if (mConfig.projections.bCompletePeriod==true)
		inc=inc/static_cast<float>(mConfig.projections.nPhaseSteps);
	else
		inc=inc/static_cast<float>(mConfig.projections.nPhaseSteps+1);

    for (size_t i=0; i<mConfig.projections.nPhaseSteps; i++)
    { // todo check this...
		sin_LUT[i]=sin(i*inc);
		cos_LUT[i]=cos(i*inc);
	}
}

kipl::base::TImage<float,3> DpcProcessor::LoadImageStack(std::string mask, size_t first, size_t stride)
{
	std::ostringstream msg;
	kipl::base::TImage<float,3> stack;
	kipl::base::TImage<float,2> slice;

	std::string filename;
	std::string ext;

    std::vector<size_t> roi = {};
	if (mConfig.projections.bUseROI==true)
		roi=mConfig.projections.nROI;

	float fDose=1.0f;

    for (size_t i=0; i<mConfig.projections.nPhaseSteps; i++)
    {
//		slice=reader.Read("",mask,first+i*stride,roi);

		if (mConfig.projections.bUseNorm==true)
			fDose=fReferenceDose/reader.GetProjectionDose("",mask,first+i*stride,mConfig.projections.nNormROI);
		else
			fDose=1.0f;
		msg.str("");
		msg<<"Dose="<<fDose;
		logger(kipl::logging::Logger::LogVerbose,msg.str());

        if (stack.Size()==0)
        {
            std::vector<size_t> dims={mConfig.projections.nPhaseSteps, slice.Size(0),slice.Size(1)};

            stack.resize(dims);
		}

		float *pSlice=slice.GetDataPtr();
		float *pDark=dark.GetDataPtr();
		ptrdiff_t j=0;
		#pragma omp parallel for
		for (j=0; j<slice.Size(); j++) 
			pSlice[j]=(pSlice[j]-pDark[j])*fDose;


		float *pStack=stack.GetDataPtr()+i;
		pSlice=slice.GetDataPtr(); // Re-get the pointer since it has changed during spotcleaning;
		ptrdiff_t k=0;
		
        for (j=0 ; j<slice.Size(); j++)
        {
			pStack[k]=pSlice[j];
			k+=mConfig.projections.nPhaseSteps;
		}
	}

	return stack;
}

kipl::base::TImage<complex<float> ,2> DpcProcessor::ComputeHarmonicImage(kipl::base::TImage<float,3> imagestack)
{
    auto dims=imagestack.dims();

    kipl::base::TImage<complex<float>, 2> result({dims[1],dims[2]});

#pragma omp parallel 
	{
        complex<float> * pResult=result.GetDataPtr();
        float *pStack= imagestack.GetDataPtr();
        #pragma omp for
        for (int i=0; i<static_cast<int>(result.Size()); i++)
        {
            float real=0.0f;
            float imag=0.0f;
            size_t sidx=i*mConfig.projections.nPhaseSteps;
            for (size_t j=0; j<mConfig.projections.nPhaseSteps; j++, sidx++)
            {
                real+=cos_LUT[j] * pStack[sidx];
                imag-=sin_LUT[j] * pStack[sidx];
            }
            pResult[i]=complex<float>(real,imag);
        }
	}
	return result;
}

void DpcProcessor::ComputeResultImages()
{
	// Compute transmission image
	const size_t N=openbeamDC.Size();
	complex<float> *pOpenBeamDC = openbeamDC.GetDataPtr();
	complex<float> *pSampleDC   = sampleDC.GetDataPtr();

	complex<float> *pOpenBeamH = openbeamH.GetDataPtr();
	complex<float> *pSampleH   = sampleH.GetDataPtr();

    transmision.resize(sampleDC.dims());
    phasecontrast.resize(sampleDC.dims());
    darkfield.resize(sampleDC.dims());

	float *pTransmision=transmision.GetDataPtr();
	float *pPhaseContrast=phasecontrast.GetDataPtr();
	float *pDarkField=darkfield.GetDataPtr();
	#pragma omp parallel for
    for (ptrdiff_t i=0; i<N; i++)
    {
		pTransmision[i]=abs(pSampleDC[i])/abs(pOpenBeamDC[i]);
		if (mConfig.process.bUseAmplLimits) {
			if (mConfig.process.fAmplLimits[1]<pTransmision[i]) pTransmision[i]=mConfig.process.fAmplLimits[1];
			if (pTransmision[i]<mConfig.process.fAmplLimits[0]) pTransmision[i]=mConfig.process.fAmplLimits[0];
		}

        //pPhaseContrast[i]=WrapDifference(arg(pSampleH[i]),arg(pOpenBeamH[i]));
        pPhaseContrast[i]=WrapDifference(atan2(pSampleH[i].real(),pSampleH[i].imag()),atan2(pOpenBeamH[i].real(),pOpenBeamH[i].imag()));
		pDarkField[i]=abs(pSampleH[i])*abs(pOpenBeamDC[i])/(abs(pSampleDC[i]) * abs(pOpenBeamH[i]));
		if (mConfig.process.bUseDFILimits) {
			if (mConfig.process.fDFILimits[1]<pDarkField[i]) pDarkField[i]=mConfig.process.fDFILimits[1];
			if (pDarkField[i]<mConfig.process.fDFILimits[0]) pDarkField[i]=mConfig.process.fDFILimits[0];
		}
	}
}

float DpcProcessor::WrapDifference(float a, float b)
{
	// Wrap phase
	const float minang=-fPi;
	float wa = minang + fmod(a - minang, 2.0f*fPi);
	float wb = minang + fmod(b - minang, 2.0f*fPi);

	float w=wa-wb+fPi;
	float result = w - 2*fPi*floor(w/(2*fPi)) - fPi;

	return result;
}

kipl::base::TImage<float,2> DpcProcessor::ComputeVisibilityMap()
{
    kipl::base::TImage<float,2> vismap(openbeamDC.dims());

    const int N=static_cast<int>(vismap.Size());

	complex<float> *pH=openbeamH.GetDataPtr();
	complex<float> *pDC=openbeamDC.GetDataPtr();
	float *pMap=vismap.GetDataPtr();
	
	#pragma omp parallel for
	for (int i=0; i<N; i++) 
	{
		pMap[i]=2.0f*abs(pH[i])/abs(pDC[i]);	
	}

	return vismap;
}
