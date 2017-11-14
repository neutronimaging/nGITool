//
// This file is part of the recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-12-07 08:57:09 +0100 (Wed, 07 Dec 2011) $
// $Rev: 1141 $
// $Id: ProjectionReader.h 1141 2011-12-07 07:57:09Z kaestner $
//

#ifndef _PROJECTIONREADER_H_
#define _PROJECTIONREADER_H_

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
	ProjectionReader(InteractionBase *interactor=NULL);
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

	kipl::base::TImage<float,2> ReadMAT(std::string filename,  size_t const * const nCrop=NULL);
	kipl::base::TImage<float,2> ReadFITS(std::string filename, size_t const * const nCrop=NULL);
	kipl::base::TImage<float,2> ReadTIFF(std::string filename, size_t const * const nCrop=NULL);
	kipl::base::TImage<float,2> ReadPNG(std::string filename,  size_t const * const nCrop=NULL);
	bool UpdateStatus(float val, std::string msg);
	bool Aborted();

	kipl::profile::Timer timer;
	InteractionBase *m_Interactor;
};

#endif
