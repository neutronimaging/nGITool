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

	void GetImageSize(std::string path, std::string filemask, size_t number, size_t *dims);
	void GetImageSize(std::string filename, size_t * dims);
	pair<float,float> GetProjectionDose(std::string filename, size_t const * const nDoseROI);
	float GetProjectionDose(std::string path, std::string filemask, size_t number, size_t const * const nCrop);

	void Initialize() {timer.Reset();}
	double ExecTime() {return timer.ElapsedSeconds();}
	kipl::base::TImage<float,2> Read(std::string filename, size_t const * const nCrop);
protected:

    kipl::base::TImage<float,2> ReadMAT(std::string filename,  size_t const * const nCrop=nullptr);
    kipl::base::TImage<float,2> ReadFITS(std::string filename, size_t const * const nCrop=nullptr);
    kipl::base::TImage<float,2> ReadTIFF(std::string filename, size_t const * const nCrop=nullptr);
    kipl::base::TImage<float,2> ReadPNG(std::string filename,  size_t const * const nCrop=nullptr);
	bool UpdateStatus(float val, std::string msg);
	bool Aborted();

	kipl::profile::Timer timer;
	InteractionBase *m_Interactor;
};

#endif
