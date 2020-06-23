//<LICENSE>

#ifndef ESTIMATORBASE_H
#define ESTIMATORBASE_H
#include "nGIFramework_global.h"
#include <string>

#include <base/timage.h>
#include <logging/logger.h>
#include <profile/Timer.h>
#include "InteractionBase.h"
#include <complex>

#include "nGIConfig.h"

class NGIFRAMEWORKSHARED_EXPORT EstimatorBase
{
protected:
	kipl::logging::Logger logger;
public:
    EstimatorBase(std::string name, InteractionBase *interactor=nullptr);
	virtual ~EstimatorBase(void);

	virtual size_t Process(kipl::base::TImage<float,3> proj, std::map<std::string, std::string> parameters);
	virtual size_t ProcessReferences(kipl::base::TImage<float,3> proj, std::map<std::string, std::string> parameters);

	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters)=0;
	virtual int Initialize()=0;
	virtual std::map<std::string, std::string> GetParameters()=0;

	std::string ModuleName() {return m_sModuleName;}
	virtual void GetProcessedImages(kipl::base::TImage<float,2> & transmission,
			kipl::base::TImage<float,2> & phase,
			kipl::base::TImage<float,2> &darkfield);
	kipl::base::TImage<float,2> ComputeVisibilityMap();
        float ComputeVisibility(size_t *roi=nullptr);

    double ExecTime() {return timer.elapsedTime(); }
protected:
	virtual int ProcessCore(kipl::base::TImage<float,3> proj,
			std::map<std::string, std::string> parameters,
			kipl::base::TImage<std::complex<float>,2> & H0,
			kipl::base::TImage<std::complex<float>,2> &H1) = 0;

	int ComputeResidualImage(kipl::base::TImage<float,3> proj);

	kipl::base::TImage<float,3> PermuteImage(kipl::base::TImage<float,3> & img);
	void SaveHarmonicImages();

	float WrapDifference(float a, float b);
	bool UpdateStatus(float val, std::string msg);
	nGIConfig mConfig;

	std::string m_sModuleName;

	kipl::base::TImage<std::complex<float>,2> m_SampleH0;
	kipl::base::TImage<std::complex<float>,2> m_SampleH1;
	kipl::base::TImage<std::complex<float>,2> m_ReferenceH0;
	kipl::base::TImage<std::complex<float>,2> m_ReferenceH1;

	kipl::profile::Timer timer;
	bool m_bSaveHarmonicImages;

	InteractionBase *m_Interactor;
};

#endif
