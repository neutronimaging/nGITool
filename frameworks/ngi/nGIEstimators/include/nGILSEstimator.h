//<LICENSE>

#ifndef NGILSESTIMATOR_H
#define NGILSESTIMATOR_H

#include "../src/stdafx.h"
#include <EstimatorBase.h>
#include <armadillo>

class nGILSEstimator: public EstimatorBase {
public:
	nGILSEstimator();
	virtual ~nGILSEstimator();
	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters);
	virtual int Initialize();
	virtual std::map<std::string, std::string> GetParameters();
	void leastsquare(float *data, int N, complex<float> &H0, complex<float> &H1);
protected:
	virtual int ProcessCore(kipl::base::TImage<float,3> proj,
			std::map<std::string, std::string> parameters,
			kipl::base::TImage<std::complex<float>,2> & H0,
			kipl::base::TImage<std::complex<float>,2> &H1);

	void ComputeHarmonicImage(kipl::base::TImage<float,3> img,
			kipl::base::TImage<std::complex<float>,2> &H0,
			kipl::base::TImage<std::complex<float>,2> &H1);

	int PrepareKernel(float fHarmonic, int N, bool bCompletePeriod);

	size_t m_nFilterSize;
    arma::mat m_H;
    arma::mat m_Ht;

	kipl::base::TImage<float,3> m_variance;
};

#endif /* nGILSEstimator_H_ */
