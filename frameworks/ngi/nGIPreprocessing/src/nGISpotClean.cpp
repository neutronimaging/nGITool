//<LICENSE>

#ifdef _OPENMP
#include <omp.h>
#endif
//#include <emmintrin.h>
//#include <xmmintrin.h>
#include <iostream>
#include <set>
#include <limits>
#include <algorithm>
#include <iterator>

#include <base/timage.h>
#include <filters/filter.h>
#include <filters/medianfilter.h>
#include <math/mathfunctions.h>
#include <profile/Timer.h>
#include <morphology/morphology.h>
#include <morphology/morphdist.h>
#include <morphology/morphfilters.h>
#include <morphology/label.h>
#include <containers/ArrayBuffer.h>
#include <strings/miscstring.h>

#include "../include/nGISpotClean.h"
#include <strings/filenames.h>
#include <ProjectionReader.h>
#include <nGIException.h>
#include <ModuleException.h>
#include <ParameterHandling.h>
#include <base/KiplException.h>
#include <strings/string2array.h>

nGISpotClean::nGISpotClean() :
    PreprocModuleBase("nGISpotClean"),
    mark(std::numeric_limits<float>::max()),
    m_Config(""),
    m_fGamma(0.1f),
    m_fSigma(0.001f),
    m_nIterations(1),
    m_fMaxLevel(2.0f),
    m_fMinLevel(-0.1f),
    m_nMaxArea(100),
    eEdgeProcessingStyle(kipl::filters::FilterBase::EdgeMirror)
{

}

nGISpotClean::~nGISpotClean() {
}

int nGISpotClean::Configure(nGIConfig config, std::map<std::string, std::string> parameters)
{
	m_Config = config;

	m_fGamma=GetFloatParameter(parameters,"gamma");
	m_fSigma=GetFloatParameter(parameters,"sigma");
	m_nIterations=GetIntParameter(parameters,"iterations");
	m_fMaxLevel=GetFloatParameter(parameters,"maxlevel");
	m_fMinLevel=GetFloatParameter(parameters,"minlevel");
	m_nMaxArea=GetIntParameter(parameters,"maxarea");
	string2enum(GetStringParameter(parameters,"detectionmethod"),m_eDetectionMethod);

    Setup(m_nIterations,m_fGamma,m_fSigma,m_fMinLevel,m_fMaxLevel, m_nMaxArea, m_eDetectionMethod);

	return 0;
}

std::map<std::string, std::string> nGISpotClean::GetParameters()
{
	std::map<std::string, std::string> parameters;

	parameters["gamma"]			  = kipl::strings::value2string(m_fGamma);
	parameters["sigma"]			  = kipl::strings::value2string(m_fSigma);
	parameters["iterations"]      = kipl::strings::value2string(m_nIterations);
	parameters["maxlevel"]		  = kipl::strings::value2string(m_fMaxLevel);
	parameters["minlevel"]        = kipl::strings::value2string(m_fMinLevel);
	parameters["maxarea"]         = kipl::strings::value2string(m_nMaxArea);
	parameters["detectionmethod"] = enum2string(m_eDetectionMethod);

	return parameters;
}

int nGISpotClean::Setup(size_t iterations,
		float threshold, float width,
		float minlevel, float maxlevel,
		int maxarea,
		ImagingAlgorithms::DetectionMethod method)
{
	m_fGamma           = threshold;
	m_fSigma           = width;
	m_nIterations      = iterations;
	m_fMaxLevel        = maxlevel;
	m_fMinLevel        = minlevel;
	m_nMaxArea         = maxarea;
	m_eDetectionMethod = method;

	m_SpotClean.Setup(m_nIterations,m_fGamma,m_fSigma,m_fMinLevel,m_fMaxLevel, m_nMaxArea, m_eDetectionMethod);

	return 0;
}

int nGISpotClean::ProcessCore(kipl::base::TImage<float,2> & img, std::map<std::string, std::string> &coeff)
{
	std::ostringstream msg;
	msg.str("");
	msg<<Version();
	logger(kipl::logging::Logger::LogMessage,msg.str());

	m_SpotClean.Process(img);

	return 0;
}

int nGISpotClean::ProcessCore(kipl::base::TImage<float,3> &img, std::map<std::string,std::string> &parameters)
{
	std::ostringstream msg;
	msg.str("");
	msg<<Version();
	logger(kipl::logging::Logger::LogVerbose,msg.str());

	m_SpotClean.Process(img);
	return 0;
}


double nGISpotClean::ChangeStatistics(kipl::base::TImage<float,2> img)
{
	const size_t N=img.Size();
	size_t cnt=0;
	float *pData=img.GetDataPtr();
	for (size_t i=0; i<N; i++) {
		cnt+=(pData[i]!=0.0f);
	}
	return static_cast<double>(cnt)/static_cast<double>(N);
}

kipl::base::TImage<float,2> nGISpotClean::DetectionImage(kipl::base::TImage<float,2> img, size_t dims)
{
	return m_SpotClean.DetectionImage(img,m_eDetectionMethod,dims);
}

kipl::base::TImage<float,2> nGISpotClean::DetectionImage(kipl::base::TImage<float,2> img)
{
	kipl::base::TImage<float,2> det_img=m_SpotClean.DetectionImage(img);

	return det_img;
}
