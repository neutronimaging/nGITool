//<LICENSE>

#ifndef NGIENGINE_H
#define NGIENGINE_H
#include "nGIFramework_global.h"
#include <list>
#include "PreprocModuleBase.h"
#include "EstimatorBase.h"
#include "ProjectionReader.h"
//#include "ReconHelpers.h"
#include "ModuleItem.h"
#include "InteractionBase.h"

#include <logging/logger.h>
#include <string>

class NGIFRAMEWORKSHARED_EXPORT nGIEngine
{
protected:
	kipl::logging::Logger logger;

public:
    nGIEngine(std::string name="nGIEngine", InteractionBase *interactor=nullptr);

	size_t AddPreProcModule(ModuleItem *module);
	void SetEstimator(EstimatorItem *module);
	int Run();
	kipl::base::TImage<float,3> RunPreprocess(size_t *roi, std::string sLastModule);
	void Preprocess(kipl::base::TImage<float,3> &img, std::string sLastModule);
	void SetConfig(nGIConfig & config) ;
    nGIConfig & GetConfig() { return m_Config; }

	bool Serialize(size_t index);

	void GetResults(kipl::base::TImage<float,2> & trans,
			kipl::base::TImage<float,2> & phase,
			kipl::base::TImage<float,2> & dark,
			kipl::base::TImage<float,2> & vis);

    void Progress(size_t *blocks, float *blockProgress, float *overallProgress)
    {// todo
	}

	kipl::base::TImage<float,3> & GetProjections() {return projections;}

	void OscillationPlot(float *axis, float *proj, float *ref);
    float Visibility(const std::vector<size_t> &roi);

	virtual ~nGIEngine(void);
protected:
	void LoadReferences(std::string sLastModule);
	void SubtractDark(kipl::base::TImage<float,3> & img);
	void SaveDataInfo();
	bool UpdateProgress(float val, std::string msg);
	void Done();
	nGIConfig m_Config;

	ProjectionReader m_ProjectionReader;             //!< Instance of the projection reader
	std::list<ModuleItem *> m_PreprocList;
	EstimatorItem * m_Estimator;

	std::map<std::string, std::string> m_ProcCoefficients;

	kipl::base::TImage<float,3> projections;
	kipl::base::TImage<float,3> references;
	kipl::base::TImage<float,2> darkcurrent;
	float darkdose;
	size_t nProcessedProjections;					//!< Counts the number of processed projections for the progress monitor
	bool m_bCancel;									//!< Cancel flag if true the reconstruction process will terminate

	InteractionBase *m_Interactor;
};

#endif
