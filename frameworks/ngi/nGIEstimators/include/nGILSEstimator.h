/*
 * nGILSEstimator.h
 *
 *  Created on: Apr 12, 2012
 *      Author: kaestner
 */

#ifndef NGILSESTIMATOR_H_
#define NGILSESTIMATOR_H_

#include "../src/stdafx.h"
#include <EstimatorBase.h>
#include <tnt.h>
#include <jama_lu.h>

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
	TNT::Array2D<float> m_H;
	TNT::Array2D<float> m_Ht;
	JAMA::LU<float> *m_LU;

	kipl::base::TImage<float,3> m_variance;
};

#endif /* nGILSEstimator_H_ */
