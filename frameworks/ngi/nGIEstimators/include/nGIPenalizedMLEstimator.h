//<LICENSE>

#ifndef nGIPenalizedMLEstimator_H
#define nGIPenalizedMLEstimator_H

#include "../src/stdafx.h"
#include <EstimatorBase.h>
#include <armadillo>

class nGIPenalizedMLEstimator: public EstimatorBase
{
public:
	nGIPenalizedMLEstimator();
	virtual ~nGIPenalizedMLEstimator();
	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters);
	virtual int Initialize();
	virtual std::map<std::string, std::string> GetParameters();
	void blue(float *data, float *var, int N, complex<float> &H0, complex<float> &H1);

protected:
	virtual int ProcessCore(kipl::base::TImage<float,3> proj,
			std::map<std::string, std::string> parameters,
			kipl::base::TImage<std::complex<float>,2> & H0,
			kipl::base::TImage<std::complex<float>,2> &H1);

	void ComputeHarmonicImage(kipl::base::TImage<float,3> img,
			kipl::base::TImage<std::complex<float>,2> &H0,
			kipl::base::TImage<std::complex<float>,2> &H1);

	kipl::base::TImage<float,2> BoxFilter(kipl::base::TImage<float,2> img, size_t dim);
	void ComputeVarianceImages(kipl::base::TImage<float,3> img);
	void PenalizeVariance(float *pData, ptrdiff_t N);

	int PrepareKernel(float fHarmonic, int N, bool bCompletePeriod);

	size_t m_nFilterSize;
    arma::mat m_H;
    arma::mat m_Ht;
	float m_fPenaltyThreshold;

	kipl::base::TImage<float,3> m_variance;
};

#endif /* nGIPenalizedMLEstimator_H_ */
