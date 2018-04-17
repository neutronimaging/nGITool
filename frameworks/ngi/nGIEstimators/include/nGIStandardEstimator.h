/*
 * nGILogNorm.h
 *
 *  Created on: Mar 15, 2012
 *      Author: kaestner
 */

#ifndef NGISTDEST_H_
#define NGISTDEST_H_
#include "../src/stdafx.h"
#include <EstimatorBase.h>
#include <complex>

class nGIStandardEstimator: public EstimatorBase {
public:
	nGIStandardEstimator();
	virtual ~nGIStandardEstimator();
	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters);
	virtual int Initialize();
	virtual std::map<std::string, std::string> GetParameters();
protected:
	virtual int ProcessCore(kipl::base::TImage<float,3> proj,
			std::map<std::string, std::string> parameters,
			kipl::base::TImage<std::complex<float>,2> & H0,
			kipl::base::TImage<std::complex<float>,2> &H1);
	int PrepareLUT(int nHarmonic);
	kipl::base::TImage<complex<float> ,2> ComputeHarmonicImage(kipl::base::TImage<float,3> imagestack);

	float *sin_LUT;
	float *cos_LUT;

};

#endif /* NGILOGNORM_H_ */
