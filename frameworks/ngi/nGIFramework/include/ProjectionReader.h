//<LICENSE>

#ifndef PROJECTIONREADER_H
#define PROJECTIONREADER_H

#include "nGIFramework_global.h"
#include <string>
#include <map>
#include <base/timage.h>
#include <logging/logger.h>
#include <profile/Timer.h>
#include "nGIConfig.h"
#include "InteractionBase.h"

/// \brief Reads the phase step projections into a 3D image
class NGIFRAMEWORKSHARED_EXPORT ProjectionReader
{
	kipl::logging::Logger logger;

public:
    ProjectionReader(InteractionBase *interactor=nullptr);
	~ProjectionReader(void);

	kipl::base::TImage<float,3> Read(nGIConfig config,std::string filemask, size_t firstindex, size_t stride,size_t index, std::map<std::string, std::string>  &parameters);

    std::vector<size_t> GetImageSize(const std::string & path, const std::string & filemask, size_t number);
    std::vector<size_t> GetImageSize(std::string filename);
    pair<float,float> GetProjectionDose(std::string filename, const std::vector<size_t> &nDoseROI);
    float GetProjectionDose(std::string path, std::string filemask, size_t number, std::vector<size_t> & nCrop);

    void Initialize() {timer.reset();}
    double ExecTime() {return timer.cumulativeTime();}
    kipl::base::TImage<float,2> Read(std::string filename, const std::vector<size_t> &nCrop);
protected:

    kipl::base::TImage<float,2> ReadFITS(std::string filename, const std::vector<size_t> &nCrop={});
    kipl::base::TImage<float,2> ReadTIFF(std::string filename, const std::vector<size_t> &nCrop={});
    kipl::base::TImage<float,2> ReadPNG(std::string filename,  const std::vector<size_t> &nCrop={});
	bool UpdateStatus(float val, std::string msg);
	bool Aborted();

	kipl::profile::Timer timer;
	InteractionBase *m_Interactor;
};

#endif
