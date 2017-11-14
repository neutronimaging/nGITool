//
// This file is part of the recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-12-06 17:38:48 +0100 (Tue, 06 Dec 2011) $
// $Rev: 1136 $
// $Id: EstimatorBase.h 1136 2011-12-06 16:38:48Z kaestner $
//

#ifndef __EstimatorBase_H
#define __EstimatorBase_H
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
	EstimatorBase(std::string name, InteractionBase *interactor=NULL);
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
    float ComputeVisibility(size_t *roi=NULL);

	double ExecTime() {return timer.ElapsedSeconds(); }
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
