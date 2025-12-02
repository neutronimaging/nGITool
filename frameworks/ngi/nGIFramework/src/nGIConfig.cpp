//<LICENSE>

#include "stdafx.h"
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include "../include/nGIConfig.h"
#include "../include/nGIException.h"
#include <libxml/xmlreader.h>
#include <strings/miscstring.h>
#include <strings/string2array.h>

nGIConfig::cProjections::cProjections() :
    nDims(2,0UL),
    sProjectionMask("./fs_####.fits"),
    sReferenceMask("./ob_####.fits"),
    sDarkMask("./dc_####.fits"),
    nDarkCnt(5),
	sDestPath("./"),
	sDestMask("result_####.tif"),
	nPhaseSteps(9),
	nFilesCnt(1),
    nFilesPerStep(1),
	nFileStride(1),
	nRefFileStride(1),
	nFirstIndex(1),
	nRefFirstIndex(1),
	nDarkFirstIndex(1),
	bCompletePeriod(true),
	fPeriods(1.0f),
	bUseROI(false),
    nROI(4,0UL),
    bUseNorm(false),
    nNormROI(4,0UL)
{
}

nGIConfig::cProjections::cProjections(const nGIConfig::cProjections & a) :
    nDims(a.nDims),
    sProjectionMask(a.sProjectionMask),
    sReferenceMask(a.sReferenceMask),
	sDarkMask(a.sDarkMask),
	nDarkCnt(a.nDarkCnt),
	sDestPath(a.sDestPath),
	sDestMask(a.sDestMask),
	nPhaseSteps(a.nPhaseSteps),
	nFilesCnt(a.nFilesCnt),
    nFilesPerStep(a.nFilesPerStep),
	nFileStride(a.nFileStride),
	nRefFileStride(a.nRefFileStride),
	nFirstIndex(a.nFirstIndex),
	nRefFirstIndex(a.nRefFirstIndex),
	nDarkFirstIndex(a.nDarkFirstIndex),
	bCompletePeriod(a.bCompletePeriod),
	fPeriods(a.fPeriods),
	bUseROI(a.bUseROI),
    nROI(a.nROI),
    bUseNorm(a.bUseNorm),
    nNormROI(a.nNormROI)
{
}

nGIConfig::cProjections & nGIConfig::cProjections::operator=(const nGIConfig::cProjections &a)
{
    nDims           = a.nDims;
    sProjectionMask = a.sProjectionMask;
    sReferenceMask  = a.sReferenceMask;
    sDarkMask       = a.sDarkMask;
    nDarkCnt        = a.nDarkCnt;
    sDestPath       = a.sDestPath;
    sDestMask       = a.sDestMask;
    nPhaseSteps     = a.nPhaseSteps;
    nFilesCnt       = a.nFilesCnt;
    nFilesPerStep   = a.nFilesPerStep;
    nFileStride     = a.nFileStride;
    nRefFileStride  = a.nRefFileStride;
    nFirstIndex     = a.nFirstIndex;
    nRefFirstIndex  = a.nRefFirstIndex;
    nDarkFirstIndex = a.nDarkFirstIndex;
    bCompletePeriod = a.bCompletePeriod;
    fPeriods        = a.fPeriods;
    bUseROI         = a.bUseROI;
    nROI            = a.nROI;
    bUseNorm        = a.bUseNorm;
    nNormROI        = a.nNormROI;

	return *this;
}


std::string nGIConfig::cProjections::WriteXML(size_t indent)
{
	using namespace std;
	ostringstream str;
	str<<setw(indent)  <<" "<<"<projections>"<<std::endl;
        str<<setw(indent+4)<<" "<<"<projmask>"<<sProjectionMask<<"</projmask>\n";
        str<<setw(indent+4)<<" "<<"<refmask>"<<sReferenceMask<<"</refmask>\n";
		str<<setw(indent+4)<<" "<<"<darkmask>"<<sDarkMask<<"</darkmask>\n";
		str<<setw(indent+4)<<" "<<"<darkcnt>"<<kipl::strings::value2string(nDarkCnt)<<"</darkcnt>\n";
		str<<setw(indent+4)<<" "<<"<destpath>"<<sDestPath<<"</destpath>\n";
		str<<setw(indent+4)<<" "<<"<destmask>"<<sDestMask<<"</destmask>\n";
		str<<setw(indent+4)<<" "<<"<phasesteps>"<<kipl::strings::value2string(nPhaseSteps)<<"</phasesteps>\n";
		str<<setw(indent+4)<<" "<<"<filescnt>"<<kipl::strings::value2string(nFilesCnt)<<"</filescnt>\n";
        str<<setw(indent+4)<<" "<<"<filesperstep>"<<kipl::strings::value2string(nFilesPerStep)<<"</filesperstep>\n";
		str<<setw(indent+4)<<" "<<"<filestride>"<<kipl::strings::value2string(nFileStride)<<"</filestride>\n";
		str<<setw(indent+4)<<" "<<"<reffilestride>"<<kipl::strings::value2string(nRefFileStride)<<"</reffilestride>\n";
		str<<setw(indent+4)<<" "<<"<firstindex>"<<kipl::strings::value2string(nFirstIndex)<<"</firstindex>\n";
		str<<setw(indent+4)<<" "<<"<reffirstindex>"<<kipl::strings::value2string(nRefFirstIndex)<<"</reffirstindex>\n";
		str<<setw(indent+4)<<" "<<"<darkfirstindex>"<<kipl::strings::value2string(nDarkFirstIndex)<<"</darkfirstindex>\n";
		str<<setw(indent+4)<<" "<<"<completeperiod>"<<kipl::strings::bool2string(bCompletePeriod)<<"</completeperiod>\n";
		str<<setw(indent+4)<<" "<<"<periods>"<<kipl::strings::value2string(fPeriods)<<"</periods>\n";
		str<<setw(indent+4)<<" "<<"<useroi>"<<kipl::strings::bool2string(bUseROI)<<"</useroi>\n";
		str<<setw(indent+4)<<" "<<"<usenorm>"<<kipl::strings::bool2string(bUseNorm)<<"</usenorm>\n";
		str<<setw(indent+4)  <<" "<<"<roi>"<<nROI[0]<<" "<<nROI[1]<<" "<<nROI[2]<<" "<<nROI[3]<<"</roi>\n";
		str<<setw(indent+4)  <<" "<<"<normroi>"<<nNormROI[0]<<" "<<nNormROI[1]<<" "<<nNormROI[2]<<" "<<nNormROI[3]<<"</normroi>\n";
	str<<setw(indent)  <<" "<<"</projections>"<<std::endl;

	return str.str();
}

nGIConfig::cProcessing::cProcessing() :
    bComputeAmplitude(true),
    bComputeDPC(true),
    bComputeDFI(true),
    bComputeVisibilty(false),
    nVisibilityROI(4,0UL),
    bUseAmplLimits(false),
    fAmplLimits({-0.1f, 1.25f}),
    bUseDFILimits(false),
    fDFILimits({0.0f,1.0f}),
    bSerialize(false)
{
}

nGIConfig::cProcessing::cProcessing(const cProcessing & a) :
    bComputeAmplitude(a.bComputeAmplitude),
    bComputeDPC(a.bComputeDPC),
    bComputeDFI(a.bComputeDFI),
    bComputeVisibilty(a.bComputeVisibilty),
    nVisibilityROI(a.nVisibilityROI),
    bUseAmplLimits(a.bUseAmplLimits),
    fAmplLimits(a.fAmplLimits),
    bUseDFILimits(a.bUseDFILimits),
    fDFILimits(a.fDFILimits),
    bSerialize(a.bSerialize)
{
}

nGIConfig::cProcessing & nGIConfig::cProcessing::operator=(const cProcessing &a)
{
	bComputeAmplitude = a.bComputeAmplitude;
	bComputeDPC       = a.bComputeDPC;
	bComputeDFI       = a.bComputeDFI;
	bComputeVisibilty = a.bComputeVisibilty;
    nVisibilityROI    = a.nVisibilityROI;
	bUseAmplLimits    = a.bUseAmplLimits;
    fAmplLimits       = a.fAmplLimits;
	bUseDFILimits     = a.bUseDFILimits;
    fDFILimits        = a.fDFILimits;
	bSerialize        = a.bSerialize;

	return *this;
}

std::string nGIConfig::cProcessing::WriteXML(size_t indent)
{
	using namespace std;
	ostringstream str;
	str<<setw(indent)  <<" "<<"<processing>\n";
		str<<setw(indent+4)<<" "<<"<computeamplitude>"<<kipl::strings::bool2string(bComputeAmplitude)<<"</computeamplitude>\n";
		str<<setw(indent+4)<<" "<<"<computedfi>"<<kipl::strings::bool2string(bComputeDFI)<<"</computedfi>\n";
		str<<setw(indent+4)<<" "<<"<computedpc>"<<kipl::strings::bool2string(bComputeDPC)<<"</computedpc>\n";
		str<<setw(indent+4)<<" "<<"<computevis>"<<kipl::strings::bool2string(bComputeVisibilty)<<"</computevis>\n";
        str<<setw(indent+4)<<" "<<"<visibilityroi>"<<nVisibilityROI[0]<<" "<<nVisibilityROI[1]<<" "<<nVisibilityROI[2]<<" "<<nVisibilityROI[3]<<"</visibilityroi>\n";
		str<<setw(indent+4)<<" "<<"<useampllimits>"<<kipl::strings::bool2string(bUseAmplLimits)<<"</useampllimits>\n";
		str<<setw(indent+4)<<" "<<"<ampllimits>"<<fAmplLimits[0]<<" "<<fAmplLimits[1]<<"</ampllimits>\n";
		str<<setw(indent+4)<<" "<<"<usedfilimits>"<<kipl::strings::bool2string(bUseDFILimits)<<"</usedfilimits>\n";
		str<<setw(indent+4)<<" "<<"<dfilimits>"<<fDFILimits[0]<<" "<<fDFILimits[1]<<"</dfilimits>\n";
	str<<setw(indent)  <<" "<<"</processing>\n";

	return str.str();
}

void nGIConfig::ParseProcess(xmlTextReaderPtr reader)
{
	std::ostringstream msg;
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1)
    {
        if (xmlTextReaderNodeType(reader)==1)
        {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);

	        value = xmlTextReaderConstValue(reader);
            if ( name==nullptr )
            {
	            throw nGIException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }

            if ( value!=nullptr )
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";

	        sName=reinterpret_cast<const char *>(name);

			if (sName=="computeamplitude") process.bComputeAmplitude = kipl::strings::string2bool(sValue);
			if (sName=="computedpc")    process.bComputeDPC          = kipl::strings::string2bool(sValue);
			if (sName=="computedfi")    process.bComputeDFI          = kipl::strings::string2bool(sValue);
			if (sName=="computevis")    process.bComputeVisibilty    = kipl::strings::string2bool(sValue);
			if (sName=="usedfilimits")  process.bUseDFILimits        = kipl::strings::string2bool(sValue);
			if (sName=="dfilimits")		kipl::strings::String2Array(sValue.c_str(),process.fDFILimits,2);
			if (sName=="useampllimits")  process.bUseAmplLimits      = kipl::strings::string2bool(sValue);
			if (sName=="ampllimits")	kipl::strings::String2Array(sValue.c_str(),process.fAmplLimits,2);
            if (sName=="visibilityroi")	kipl::strings::String2Array(sValue.c_str(),process.nVisibilityROI,4);
    	}
		ret = xmlTextReaderRead(reader);
		if (xmlTextReaderDepth(reader)<depth)
			ret=0;
    }

}

nGIConfig::nGIConfig(const std::string &appPath) :
    ConfigBase("nGIConfig",appPath),
    estimator(appPath,"estimators")

{
#ifdef _MSCVER_
	estimator.m_sSharedObject="nGIEstimators.dll";
#else
	estimator.m_sSharedObject="/opt/ngi/libnGIEstimators.so";
#endif
	estimator.m_sModule="blueestimator";
	estimator.m_bActive=true;

	std::map<std::string,std::string> parameters;

}

nGIConfig::nGIConfig(nGIConfig &c) :
	ConfigBase(c),
	projections(c.projections),
	process(c.process),
	estimator(c.estimator)
{
}

nGIConfig::~nGIConfig()
{
}

nGIConfig & nGIConfig::operator=(nGIConfig &c)
{
	UserInformation=c.UserInformation;
	projections=c.projections;
	process=c.process;
	modules = c.modules;
	estimator = c.estimator;

	return *this;
}

std::string nGIConfig::WriteXML()
{
	std::ostringstream str;

	int indent=4;
	str<<"<ngi>\n";
		str<<UserInformation.WriteXML(indent);
		str<<projections.WriteXML(indent);
		str<<process.WriteXML(indent);

		str<<std::setw(indent)<<" "<<"<processchain>\n";
        if (!modules.empty())
        {
			str<<std::setw(indent+4)<<" "<<"<preprocessing>\n";
			std::list<ModuleConfig>::iterator it;

            for (auto &module : modules)
            {
                str<<module.WriteXML(indent+8);
			}
			str<<std::setw(indent+4)<<" "<<"</preprocessing>\n";
		}
		str<<std::setw(indent+4)<<" "<<"<estimator>\n";
		str<<estimator.WriteXML(indent+8);
		str<<std::setw(indent+4)<<" "<<"</estimator>\n";

		str<<std::setw(indent)<<" "<<"</processchain>\n";


		str<<"</ngi>"<<std::endl;

		return str.str();
}

void nGIConfig::ParseConfig(xmlTextReaderPtr reader, std::string sName)
{
	if (sName=="projections")
		ParseProjections(reader);

	if (sName=="processing")
		ParseProcess(reader);

}

void nGIConfig::ParseProjections(xmlTextReaderPtr reader)
{
	std::ostringstream msg;
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);
    while (ret == 1)
    {
        if (xmlTextReaderNodeType(reader)==1)
        {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);

	        value = xmlTextReaderConstValue(reader);
            if ( name==nullptr )
            {
	            throw nGIException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }
            if ( value!=nullptr )
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";

	        sName=reinterpret_cast<const char *>(name);

            if (sName=="refmask")        projections.sReferenceMask = sValue;
            if (sName=="projmask")       projections.sProjectionMask= sValue;
            if (sName=="darkmask")       projections.sDarkMask      = sValue;
            if (sName=="destpath")       projections.sDestPath      = sValue;
            if (sName=="destmask")       projections.sDestMask      = sValue;
            if (sName=="phasesteps")     projections.nPhaseSteps    = atoi(sValue.c_str());
            if (sName=="filescnt")       projections.nFilesCnt      = atoi(sValue.c_str());
            if (sName=="filesperstep")   projections.nFilesPerStep  = atoi(sValue.c_str());
            if (sName=="filestride")     projections.nFileStride    = atoi(sValue.c_str());
            if (sName=="firstindex")     projections.nFirstIndex    = atoi(sValue.c_str());
            if (sName=="reffirstindex")  projections.nRefFirstIndex = atoi(sValue.c_str());
            if (sName=="reffilestride")  projections.nRefFileStride = atoi(sValue.c_str());
			if (sName=="darkfirstindex") projections.nDarkFirstIndex = atoi(sValue.c_str());
            if (sName=="darkcnt")		 projections.nDarkCnt = atoi(sValue.c_str());
			if (sName=="completeperiod") projections.bCompletePeriod = kipl::strings::string2bool(sValue);
            if (sName=="periods")        projections.fPeriods       = atof(sValue.c_str());
            if (sName=="useroi")         projections.bUseROI        = kipl::strings::string2bool(sValue);
            if (sName=="roi")            kipl::strings::String2Array(sValue,projections.nROI,4);
            if (sName=="usenorm")        projections.bUseNorm       = kipl::strings::string2bool(sValue);
            if (sName=="normroi")        kipl::strings::String2Array(sValue,projections.nNormROI,4);

    	}
		ret = xmlTextReaderRead(reader);
		if (xmlTextReaderDepth(reader)<depth)
			ret=0;
    }
}

void nGIConfig::ParseProcessChain(xmlTextReaderPtr reader)
{
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1)
    {
        if (xmlTextReaderNodeType(reader)==1)
        {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);

	        value = xmlTextReaderConstValue(reader);
            if ( name==nullptr )
            {
	            throw nGIException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }

            if ( value!=nullptr )
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";
	        sName=reinterpret_cast<const char *>(name);

            if (sName=="preprocessing")
            {
				logger(kipl::logging::Logger::LogVerbose,"Parsing pre-processing modules");
				int depth2=xmlTextReaderDepth(reader);
                while (ret == 1)
                {
                    if (xmlTextReaderNodeType(reader)==1)
                    {
						name = xmlTextReaderConstName(reader);
						ret=xmlTextReaderRead(reader);

						value = xmlTextReaderConstValue(reader);
                        if ( name==nullptr )
                        {
							throw nGIException("Unexpected contents in parameter file",__FILE__,__LINE__);
						}
                        if ( value!=nullptr )
        					sValue=reinterpret_cast<const char *>(value);
						else
        					sValue="Empty";
						sName=reinterpret_cast<const char *>(name);
						if (sName=="module") {
                            ModuleConfig module(m_sApplicationPath,"estimators");
							module.ParseModule(reader);
							modules.push_back(module);
						}
					}
					ret = xmlTextReaderRead(reader);
					if (xmlTextReaderDepth(reader)<depth2)
						ret=0;
				}
			}
            if (sName=="estimator")
            {
				logger(kipl::logging::Logger::LogVerbose,"Parsing the estimator");
				estimator.ParseModule(reader);
			}

		}
		ret = xmlTextReaderRead(reader);
		if (xmlTextReaderDepth(reader)<depth)
    		ret=0;
	}
}

std::string nGIConfig::SanitySlicesCheck()
{
    return "";
}

std::string nGIConfig::SanityMessage(bool msg)
{
    return "";
}
