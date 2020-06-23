//<LICENSE>

#include "../include/nGILogNorm.h"
#include <strings/filenames.h>
#include <ProjectionReader.h>
#include <nGIException.h>
#include <ModuleException.h>
#include <ParameterHandling.h>
#include <base/KiplException.h>
#include <strings/string2array.h>

nGILogNorm::nGILogNorm() : m_Config("")
{
}

nGILogNorm::~nGILogNorm() {
}

int nGILogNorm::Configure(nGIConfig config, std::map<std::string, std::string> parameters)
{

	return 0;
}

std::map<std::string, std::string> nGILogNorm::GetParameters()
{
	std::map<std::string, std::string> parameters;

	return parameters;
}

int nGILogNorm::ProcessCore(kipl::base::TImage<float,3> &img, std::map<std::string,std::string> &parameters)
{
	if ((img.Size(0)!=ob.Size(0)) || (img.Size(1)!=ob.Size(1)) || (img.Size(2)!=ob.Size(2)))
		throw nGIException("Size mismatch between projection and reference data", __FILE__,__LINE__);

	float *pImg;
	float *pRef;
	float *pDC=dc.GetDataPtr();
	float diff;

	float *dose = new float[m_nSliceSize];

	if (m_Config.projections.bUseNorm) {
		kipl::strings::String2Array(GetStringParameter(parameters, "dose") , dose, m_Config.projections.nPhaseSteps);
		for (size_t i=0; i<img.Size(2); i++)
			dose[i]=log(dose[i]);
	}
	else {
		memset(dose,0,sizeof(float)*m_nSliceSize);
	}

	for (size_t i=0; i<img.Size(2); i++ ) {
		pImg=img.GetLinePtr(0,i);
		pRef=ob.GetLinePtr(0,i);

		for (size_t j=0; j<m_nSliceSize; j++) {
			diff=pImg[j]-pDC[j];
			pImg[j] = diff<=0.0f ? pRef[j] : pRef[j]-log(diff)+dose[i];
		}
	}

	delete [] dose;
	return 0;
}

void nGILogNorm::LoadReferenceImages()
{
	ostringstream msg;
	kipl::base::TImage<float,2> img;

    std::vector<size_t> roi = {} ;
    if (m_Config.projections.bUseROI)
        roi = m_Config.projections.nROI;

	std::map<std::string, std::string>  parameters;
	std::string fname;
	std::string ext;
	ProjectionReader reader;
	try {
		if (m_Config.projections.nDarkCnt) {
            kipl::strings::filenames::MakeFileName(m_Config.projections.sDarkMask,
					m_Config.projections.nDarkFirstIndex, fname, ext, '#','0');
			dc=reader.Read(fname,roi);

			for (size_t idx=1; idx<m_Config.projections.nDarkCnt; idx++) {
                kipl::strings::filenames::MakeFileName(m_Config.projections.sDarkMask,
						m_Config.projections.nDarkFirstIndex+idx, fname, ext, '#','0');
				img=reader.Read(fname,roi);
				dc+=img;
			}
		}
	}
	catch (nGIException & e) {
		msg<<"nGIException while dark image reading: "<<e.what();
	}
	catch (ModuleException & e) {
		msg<<"ModuleException while dark image reading: "<<e.what();
	}

	catch (kipl::base::KiplException & e) {
		msg<<"KiplException while dark image reading: "<<e.what();
	}

	catch (std::exception & e) {
		msg<<"STL exception while dark image reading: "<<e.what();
	}
	catch (...) {
		msg<<"Unknown exception while dark image reading.";
	}
	m_nSliceSize=dc.Size();

	if (m_Config.projections.nPhaseSteps) {
		float *fOBDose = new float[m_Config.projections.nPhaseSteps];
		try {
            ob=reader.Read(m_Config , m_Config.projections.sReferenceMask, m_Config.projections.nRefFirstIndex,m_Config.projections.nRefFileStride, 0UL, parameters);
			kipl::strings::String2Array(GetStringParameter(parameters, "dose") , fOBDose, m_Config.projections.nPhaseSteps);
		}
		catch (nGIException & e) {
			msg<<"nGIException while open beam image reading: "<<e.what();
		}
		catch (ModuleException & e) {
			msg<<"ModuleException while open beam image reading: "<<e.what();
		}

		catch (kipl::base::KiplException & e) {
			msg<<"KiplException while open beam image reading: "<<e.what();
		}

		catch (std::exception & e) {
			msg<<"STL exception while open beam image reading: "<<e.what();
		}
		catch (...) {
			msg<<"Unknown exception while open beam image reading.";
		}

		if ((ob.Size(0)!=dc.Size(0)) || (ob.Size(1)!=dc.Size(1)))
			throw nGIException("Reference image size mismatch",__FILE__,__LINE__);

		float *pDC=dc.GetDataPtr();


		for (size_t i=0; i<m_Config.projections.nPhaseSteps; i++) {
			float *pOB=ob.GetLinePtr(0,i);
			float fLogDose=log(fOBDose[i]);
			for (size_t j=0; j<m_nSliceSize; j++) {
				float diff=pOB[j]-pDC[j];
				pOB[j]= 0<diff ? log(diff)- fLogDose : -fLogDose;
			}

		}
		delete [] fOBDose;
	}


}
