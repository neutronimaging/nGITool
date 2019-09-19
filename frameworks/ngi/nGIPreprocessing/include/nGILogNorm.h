//<LICENSE>

#ifndef NGILOGNORM_H_
#define NGILOGNORM_H_
#include "ngipreprocessing_global.h"
#include <PreprocModuleBase.h>

class NGIPREPROCESSINGSHARED_EXPORT nGILogNorm: public PreprocModuleBase {
public:
	nGILogNorm();
	virtual ~nGILogNorm();
	virtual int Configure(nGIConfig config, std::map<std::string, std::string> parameters);
	virtual std::map<std::string, std::string> GetParameters();
protected:
	virtual int ProcessCore(kipl::base::TImage<float,3> &img, std::map<std::string,std::string> &parameters);
	void LoadReferenceImages();

	nGIConfig m_Config;

	size_t m_nSliceSize;

	kipl::base::TImage<float,3> ob;
	kipl::base::TImage<float,2> dc;
};

#endif /* NGILOGNORM_H_ */
