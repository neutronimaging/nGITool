//<LICENSE>

#ifndef DPC_CONFIG_H
#define DPC_CONFIG_H

#include "nGIFramework_global.h"
#include <string>
#include <logging/logger.h>
#include <ConfigBase.h>

class NGIFRAMEWORKSHARED_EXPORT nGIConfig : public ConfigBase
{
public:
    nGIConfig(const std::string &appPath);
	nGIConfig(nGIConfig &c);
	virtual ~nGIConfig();

	nGIConfig & operator=(nGIConfig &c);

    struct NGIFRAMEWORKSHARED_EXPORT cProjections {
		cProjections();
		cProjections(const cProjections & a);
		cProjections & operator=(const cProjections &a);
        std::vector<size_t> nDims;
        std::string sProjectionMask;
        std::string sReferenceMask;
		std::string sDarkMask;
        size_t      nDarkCnt;
		std::string sDestPath;
		std::string sDestMask;

		size_t nPhaseSteps;
		size_t nFilesCnt;
        size_t nFilesPerStep;
		size_t nFileStride;
		size_t nRefFileStride;
		size_t nFirstIndex;
		size_t nRefFirstIndex;
		size_t nDarkFirstIndex;
		bool   bCompletePeriod;
		float fPeriods;
		bool bUseROI;
        std::vector<size_t> nROI;
		bool bUseNorm;
        std::vector<size_t> nNormROI;
		std::string WriteXML(size_t indent=0);
	};

    struct NGIFRAMEWORKSHARED_EXPORT cProcessing {
		cProcessing();
		cProcessing(const cProcessing & a);
		cProcessing & operator=(const cProcessing &a);

		bool bComputeAmplitude;
		bool bComputeDPC;
		bool bComputeDFI;
		bool bComputeVisibilty;
        std::vector<size_t> nVisibilityROI;
		bool bUseAmplLimits;
        std::vector<float> fAmplLimits;
		bool bUseDFILimits;
        std::vector<float> fDFILimits;
		bool bSerialize;
		std::string WriteXML(size_t indent=0);
	};

	cProjections projections;
	cProcessing process;
	ModuleConfig estimator;

	int ParseXML(std::string fname);

	virtual std::string WriteXML();
protected:
        virtual std::string SanitySlicesCheck();
        virtual std::string SanityMessage(bool msg);
	virtual void ParseConfig(xmlTextReaderPtr reader, std::string cName);
	void ParseProjections(xmlTextReaderPtr reader);
	void ParseProcess(xmlTextReaderPtr reader);
	virtual void ParseProcessChain(xmlTextReaderPtr reader);
};

#endif /* DPC_CONFIG_H_ */
