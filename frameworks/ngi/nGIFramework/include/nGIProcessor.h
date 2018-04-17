/*
 * nGIProcessor.h
 *
 *  Created on: Mar 12, 2010
 *      Author: anders
 */

#ifndef DPCPROCESSOR_H_
#define DPCPROCESSOR_H_

#include "nGIFramework_global.h"
#include "nGIConfig.h"
#include <logging/logger.h>
#include <base/timage.h>
#include <math/compleximage.h>

#include "ProjectionReader.h"

#include <complex>
#include <map>


class DpcProcessor {
	kipl::logging::Logger logger;
	size_t nLUTsize;
public:
	DpcProcessor(size_t lutsize=128);
	virtual ~DpcProcessor();

	int Process(int frame);
	kipl::base::TImage<float,2> ComputeVisibilityMap();
	int Initialize(nGIConfig config);
	kipl::base::TImage<float,2> GetLastPhaseImage()     { return phasecontrast; }
	kipl::base::TImage<float,2> GetLastAmplitudeImage() { return transmision;   }
	kipl::base::TImage<float,2> GetLastDarkFieldImage() { return darkfield;     }
	float * GetOscillationPlot()            { return oscillation;   }
private:
	/// \brief Computes the LUT for the Nth harmonic
	/// \param nHarmonic Index of the harmonic to compute
	void PrepareLUT(size_t nHarmonic);

	/// \brief Load a stack of images and organizes them as ZXY
	/// \param mask File name mask
	/// \param first index of the first file to load
	kipl::base::TImage<float,3> LoadImageStack(std::string mask, size_t first, size_t stride=1);


	void LoadDark();

	/// \brief Computes the harmonic images from an image stack using LUT for sine and cosine
	/// \param imagestack An image containing a sequence of interferometer images organized as ZXY
	/// \returns A complex valued harmonic image
	kipl::base::TImage<complex<float> ,2> ComputeHarmonicImage(kipl::base::TImage<float,3> imagestack);

	void ComputeResultImages();
	void ComputeOscillationPlot(kipl::base::TImage<float,3> stack);

	kipl::base::TImage<float> Unwrap(kipl::base::TImage<float> imgA, kipl::base::TImage<float> imgB);
	float WrapDifference(float a, float b);
	nGIConfig mConfig;

	kipl::base::TImage<float,2> dark;
	kipl::base::TImage<complex<float>,2 > openbeamDC;
	kipl::base::TImage<complex<float>,2 > openbeamH;

	kipl::base::TImage<complex<float>,2 > sampleDC;
	kipl::base::TImage<complex<float>,2 > sampleH;

	kipl::base::TImage<float,2 > transmision;
	kipl::base::TImage<float,2 > phasecontrast;
	kipl::base::TImage<float,2 > darkfield;

	float oscillation[128];

	float *sin_LUT;
	float *cos_LUT;
	float fReferenceDose;
	ProjectionReader reader;

	std::map<std::string, size_t> extmap;

};

#endif /* DPCPROCESSOR_H_ */
