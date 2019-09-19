//<LICENSE>

#ifndef NGISPOTCLEAN_H_
#define NGISPOTCLEAN_H_
#include "ngipreprocessing_global.h"
#include <PreprocModuleBase.h>
#include <base/timage.h>
#include <filters/filterbase.h>
#include <math/LUTCollection.h>
#include <containers/ArrayBuffer.h>
#include <map>
#include <list>
#include <SpotClean.h>

class NGIPREPROCESSINGSHARED_EXPORT nGISpotClean: public PreprocModuleBase {
public:
	const float mark;
	nGISpotClean();
	virtual ~nGISpotClean();
	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters);
	virtual std::map<std::string, std::string> GetParameters();

	int Setup(size_t iterations,
			float threshold, float width,
			float minlevel, float maxlevel,
			int maxarea,
			ImagingAlgorithms::DetectionMethod method);

	kipl::base::TImage<float,2> DetectionImage(kipl::base::TImage<float,2> img, size_t dims);
	kipl::base::TImage<float,2> DetectionImage(kipl::base::TImage<float,2> img);
	double ChangeStatistics(kipl::base::TImage<float,2> img);

protected:
	virtual int ProcessCore(kipl::base::TImage<float,2> & img, std::map<std::string, std::string> &coeff);
	virtual int ProcessCore(kipl::base::TImage<float,3> &img, std::map<std::string,std::string> &parameters);

//
//	kipl::base::TImage<float,2> DetectSpots(kipl::base::TImage<float,2> img, kipl::containers::ArrayBuffer<PixelInfo2> *pixels);
//	void ExcludeLargeRegions(kipl::base::TImage<float,2> &img);
//
//	kipl::base::TImage<float,2> CleanByList(kipl::base::TImage<float,2> img);
//	kipl::base::TImage<float,2> CleanByArray(kipl::base::TImage<float,2> img,kipl::containers::ArrayBuffer<PixelInfo2> *pixels);
//
//	kipl::base::TImage<float,2> BoxFilter(kipl::base::TImage<float,2> img, size_t dim);
//	kipl::base::TImage<float,2> StdDev(kipl::base::TImage<float,2> img, size_t dim);
//
//
//	/// \brief Prepares neighborhood indexing LUT
//	/// \param dimx Length of the x-axis
//	/// \param N number of pixels in the image
//	int PrepareNeighborhood(int dimx, int N);
//
//	/// \brief Extracts the neighborhood of a pixel. Skips pixels with the value max(float)
//	/// \param pImg reference to the image matrix
//	/// \param idx Index of the pixel
//	/// \param neigborhood preallocated array to carry the extracted neighbors
//	/// \returns number of extracted pixels in the array
//	int Neighborhood(float * pImg, int idx, float * neigborhood);

	ImagingAlgorithms::SpotClean m_SpotClean;

	nGIConfig m_Config;

	float m_fGamma;
	float m_fSigma;
	int m_nIterations;
	float m_fMaxLevel;
	float m_fMinLevel;
	size_t m_nMaxArea;
	ImagingAlgorithms::DetectionMethod m_eDetectionMethod;



	kipl::filters::FilterBase::EdgeProcessingStyle eEdgeProcessingStyle;
};

#endif /* nGISpotClean_H_ */
