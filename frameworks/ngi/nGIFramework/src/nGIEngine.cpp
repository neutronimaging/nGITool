//<LICENSE>

#include "stdafx.h"
#include <iostream>
#include <ctime>

#include <logging/logger.h>
#include <base/timage.h>
#include <base/imageinfo.h>
#include <io/io_tiff.h>
#include <strings/miscstring.h>
#include <strings/filenames.h>
#include <strings/string2array.h>
#include <ParameterHandling.h>

#include "../include/nGIEngine.h"
#include "../include/nGIException.h"

nGIEngine::nGIEngine(std::string name, InteractionBase *interactor) :
	logger(name),
    m_Config(""),
	m_ProjectionReader(interactor),
    m_Estimator(nullptr),
    m_Interactor(interactor)
{
	logger(kipl::logging::Logger::LogMessage,"C'tor BackProjBase");
    if (m_Interactor!=nullptr) {
		logger(kipl::logging::Logger::LogMessage,"Got an interactor");
	}
	else {
		logger(kipl::logging::Logger::LogMessage,"An interactor was not provided");

	}
}

nGIEngine::~nGIEngine(void)
{
	std::ostringstream msg;
	while (!m_PreprocList.empty()) {
		msg.str("");
		msg<<"Removing "<<m_PreprocList.front()->GetModule()->ModuleName()<<" from the module list ("<<m_PreprocList.size()<<")";
		logger(kipl::logging::Logger::LogVerbose,msg.str());
        if (m_PreprocList.front()!=nullptr) {
			delete m_PreprocList.front();
		}
		msg.str("");
		msg<<"Removed the module ("<<m_PreprocList.size()<<")";
		logger(kipl::logging::Logger::LogVerbose,msg.str());

		m_PreprocList.pop_front();
	}

	if (m_Estimator!=NULL)
		delete m_Estimator;
}

void nGIEngine::SetConfig(nGIConfig &config)
{
	m_Config=config;
}

size_t nGIEngine::AddPreProcModule(ModuleItem *module)
{
	if (module!=NULL) {
		if (module->Valid())
			m_PreprocList.push_back(module);
	}
	else
		throw nGIException("Failed to add module",__FILE__,__LINE__);

	return m_PreprocList.size();
}

void nGIEngine::SetEstimator(EstimatorItem *module)
{
	std::ostringstream msg;
	if (module!=NULL) {
		if (module->Valid()) {
			m_Estimator=module;
			msg<<"The engine was configured with "<<m_Estimator->GetModule()->ModuleName();
			logger(kipl::logging::Logger::LogMessage,msg.str());
		}
	}
	else
		throw nGIException("Failed to add estimator module",__FILE__,__LINE__);
}

int nGIEngine::Run()
{
	std::stringstream msg;

	kipl::profile::Timer totalTimer;

	totalTimer.Tic();
	m_bCancel=false;
	int result=0;
	// Process the references
	LoadReferences("");
	// Process the images
	try
	{
		for (size_t i=0; i<m_Config.projections.nFilesCnt && (m_bCancel==false); i++) {
			projections=m_ProjectionReader.Read(m_Config,
                            m_Config.projections.sProjectionMask,
							m_Config.projections.nFirstIndex,
							m_Config.projections.nFileStride,
							i,
							m_ProcCoefficients);

			SubtractDark(projections);
			Preprocess(projections,"");
			m_Estimator->GetModule()->Process(projections,m_ProcCoefficients);
			logger(kipl::logging::Logger::LogMessage,"Estimation finished");
			if (m_Config.process.bSerialize) {
				logger(kipl::logging::Logger::LogMessage,"Serializing");
				Serialize(i);

				if (i==0) {
					logger(kipl::logging::Logger::LogMessage,"Saving log info");
					SaveDataInfo();
				}

				logger(kipl::logging::Logger::LogMessage,"Serialization done");

			}
			logger(kipl::logging::Logger::LogMessage,"Try next");
		}
	}
	catch (nGIException &e) {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (kipl::base::KiplException &e) {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (std::exception &e) {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (...) {
		msg.str("");
		msg<<"The reconstruction failed with an unknown error";
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	logger(kipl::logging::Logger::LogVerbose,"Projection processing done");
	totalTimer.Toc();
	msg.str("");
	msg<<"Totals for "<<m_Config.projections.nFilesCnt<<" slices"<<endl
        <<totalTimer<<" ("<<totalTimer.cumulativeTime()/static_cast<double>(m_Config.projections.nFilesCnt)<<" s/slice)";

	logger(kipl::logging::Logger::LogMessage,msg.str());

	return result;
}

kipl::base::TImage<float,3> nGIEngine::RunPreprocess(size_t *roi, std::string sLastModule)
{
	std::stringstream msg;

	kipl::base::TImage<float,3> img;


	try
	{
		LoadReferences(sLastModule);
		img=m_ProjectionReader.Read(m_Config,
                        m_Config.projections.sProjectionMask,
						m_Config.projections.nFirstIndex,
						m_Config.projections.nFileStride,
						0UL,
						m_ProcCoefficients);

		SubtractDark(img);
		Preprocess(img,sLastModule);
	}
	catch (nGIException &e) {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (kipl::base::KiplException &e) {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (std::exception &e) {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (...) {
		msg.str("");
		msg<<"The reconstruction failed with an unknown error";
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	return img;
}

void nGIEngine::LoadReferences(std::string sLastModule)
{
	darkdose=0.0f;
	std::ostringstream msg;
	logger(kipl::logging::Logger::LogVerbose,"Processing reference images.");
	try
	{
		kipl::base::TImage<float,2> dctmp;
		std::string fname,ext;
        std::vector<size_t> roi = {};
        if (m_Config.projections.bUseROI)
            roi = m_Config.projections.nROI;

        // Load dark current images
        for (size_t i=0; i<m_Config.projections.nDarkCnt; i++)
        {
            kipl::strings::filenames::MakeFileName(m_Config.projections.sDarkMask,
                    static_cast<int>(i+m_Config.projections.nDarkFirstIndex),
					fname,
					ext,
					'#','0');

			dctmp=m_ProjectionReader.Read(fname,roi);
            if (i==0)
            {
                darkcurrent.resize(dctmp.dims());
				darkcurrent=0.0f;
			}

			darkcurrent+=dctmp;
		}
        if (darkcurrent.Size()!=0)
        {
			darkcurrent/=m_Config.projections.nDarkCnt;
			darkdose=m_ProjectionReader.GetProjectionDose(fname,m_Config.projections.nNormROI).first;
		}

        // Read reference images
		references=m_ProjectionReader.Read(m_Config,
                        m_Config.projections.sReferenceMask,
						m_Config.projections.nRefFirstIndex,
						m_Config.projections.nRefFileStride,
						0UL,
						m_ProcCoefficients);


		SubtractDark(references);
		Preprocess(references,sLastModule);

        logger(kipl::logging::Logger::LogVerbose,"Sending reference images to process.");

		m_Estimator->GetModule()->ProcessReferences(references,m_ProcCoefficients);

	}
    catch (nGIException &e)
    {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
    catch (kipl::base::KiplException &e)
    {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
    catch (std::exception &e)
    {
		msg.str("");
		msg<<"The reconstruction failed with "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (...) {
		msg.str("");
		msg<<"The reconstruction failed with an unknown error";
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	logger(kipl::logging::Logger::LogVerbose,"Reference processing done");
}

void nGIEngine::Preprocess(kipl::base::TImage<float,3> &img, std::string sLastModule)
{
	std::stringstream msg;
	m_bCancel=false;

	std::list<ModuleItem *>::iterator it_Module;

	// Start processing
	kipl::profile::Timer timer;

	msg.str("");

	logger(kipl::logging::Logger::LogVerbose,"Starting preprocessing");
    try
    {
        for (it_Module=m_PreprocList.begin(); (it_Module!=m_PreprocList.end()) && (*it_Module)->GetModule()->ModuleName()!=sLastModule; it_Module++)
        {
			msg.str("");
			msg<<"Processing: "<<(*it_Module)->GetModule()->ModuleName();
			logger(kipl::logging::Logger::LogVerbose,msg.str());
			if (!(m_bCancel=UpdateProgress(0.0f, msg.str())))
				(*it_Module)->GetModule()->Process(img,m_ProcCoefficients);
			else
				break;
		}
	}
    catch (nGIException &e)
    {
		msg<<"Preprocessing failed with a nGI exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
    catch (kipl::base::KiplException &e)
    {
		msg<<"Preprocessing failed with a kipl exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
    catch (std::exception &e)
    {
		msg<<"Preprocessing failed with an STL exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

    if (m_bCancel==true)
    {
		logger(kipl::logging::Logger::LogVerbose,"Preprocessing was canceled by the user.");
	}
    else
    {
		logger(kipl::logging::Logger::LogVerbose,"Preprocessing finished");
	}

}

bool nGIEngine::Serialize(size_t index)
{
	std::stringstream msg;

	std::stringstream str;

	kipl::base::ImageInfo info;
	info.sArtist=m_Config.UserInformation.sOperator;
	info.sCopyright=m_Config.UserInformation.sOperator;
	info.sSoftware="Kaestner Grating Interferometer Processing Tool";
	info.sDescription=m_Config.UserInformation.sSample;

	kipl::base::TImage<float> trans;
	kipl::base::TImage<float> phase;
	kipl::base::TImage<float> dark;

	m_Estimator->GetModule()->GetProcessedImages(trans,phase,dark);

	std::string fname,ext;

    if (m_Config.process.bComputeAmplitude)
    {
		kipl::strings::filenames::MakeFileName(m_Config.projections.sDestPath+"TI_"+m_Config.projections.sDestMask,
                static_cast<int>(index),fname,ext,'#','0');
        kipl::io::WriteTIFF(trans,fname,kipl::base::Float32);
	}

    if (m_Config.process.bComputeDPC)
    {
		kipl::strings::filenames::MakeFileName(m_Config.projections.sDestPath+"DPC_"+m_Config.projections.sDestMask,
                static_cast<int>(index),fname,ext,'#','0');
        kipl::io::WriteTIFF(phase,fname,kipl::base::Float32);
	}

    if (m_Config.process.bComputeDFI)
    {
		kipl::strings::filenames::MakeFileName(m_Config.projections.sDestPath+"DFI_"+m_Config.projections.sDestMask,
				index,fname,ext,'#','0');
        kipl::io::WriteTIFF(dark,fname,kipl::base::Float32);
	}

    if (m_Config.process.bComputeVisibilty)
    {
		kipl::base::TImage<float> visibility;

		visibility=m_Estimator->GetModule()->ComputeVisibilityMap();

		kipl::strings::filenames::MakeFileName(m_Config.projections.sDestPath+"Visibility_"+m_Config.projections.sDestMask,
						index,fname,ext,'#','0');
        kipl::io::WriteTIFF(visibility,fname,kipl::base::Float32);
	}

	return false;
}

bool nGIEngine::UpdateProgress(float val, std::string msg)
{
    if (m_Interactor!=nullptr)
		return m_Interactor->SetProgress(val, msg);

	return false;
}

void nGIEngine::Done()
{
	if (m_Interactor!=NULL)
		m_Interactor->Done();
}

void nGIEngine::GetResults(kipl::base::TImage<float,2> & trans,
		kipl::base::TImage<float,2> & phase,
		kipl::base::TImage<float,2> & dark,
		kipl::base::TImage<float,2> & vis)
{
	m_Estimator->GetModule()->GetProcessedImages(trans,phase,dark);
	vis=m_Estimator->GetModule()->ComputeVisibilityMap();
}

float nGIEngine::Visibility(const std::vector<size_t> &roi)
{
    return m_Estimator->GetModule()->ComputeVisibility(roi);
}

void nGIEngine::OscillationPlot(float *axis, float *proj, float *ref)
{
	size_t N=21;
    size_t y_begin=projections.Size(1)/2-N/2;
	size_t y_end=y_begin+N;

    size_t x_begin=projections.Size(0)/2-N/2;
	size_t x_end=x_begin+N;

	for (size_t i=0; i<projections.Size(2); i++) {
		float sum_proj=0.0f;
		float sum_ref=0.0f;
		for (size_t y=y_begin; y<=y_end; y++) {
			float *pProj=projections.GetLinePtr(y,i);
			float *pRef=references.GetLinePtr(y,i);

			for (size_t x=x_begin; x<=x_end; x++) {
				sum_proj+=pProj[x];
				sum_ref+=pRef[x];
			}
		}
		proj[i]=sum_proj/(N*N);
		ref[i]=sum_ref/(N*N);
		axis[i]=static_cast<float>(i);
	}
}

void nGIEngine::SubtractDark(kipl::base::TImage<float,3> & img)
{
	if ((darkcurrent.Size(0)!=img.Size(0)) || (darkcurrent.Size(1)!=img.Size(1)))
		return;

	float dose[2048];
	if (m_Config.projections.bUseNorm==true) {
		kipl::strings::String2Array(GetStringParameter(m_ProcCoefficients,"dose"),dose,img.Size(2));
	}

	float *pDark=darkcurrent.GetDataPtr();
	for (size_t i=0; i<img.Size(2); i++) {
		float *pImg=img.GetLinePtr(0,i);
		if (m_Config.projections.bUseNorm==true) {
			float invdose= 1.0f/(dose[i]-darkdose);
			for (size_t j=0; j<darkcurrent.Size(); j++) {
				pImg[j]-=pDark[j];
				pImg[j]*=invdose;
			}
		}
		else {
			for (size_t j=0; j<darkcurrent.Size(); j++)
				pImg[j]-=pDark[j];
			}
	}
}

void nGIEngine::SaveDataInfo()
{
    std::string basename=m_Config.projections.sDestMask;

    basename=basename.substr(0,basename.find_first_of('#'));
    size_t slashpos=basename.find_last_of(kipl::strings::filenames::slash);
    if (slashpos!=basename.npos)
    {
        basename=basename.substr(slashpos);
    }
    std::string filename=m_Config.projections.sDestPath+basename+"_info.csv";
	logger(kipl::logging::Logger::LogMessage,filename);
	ofstream reportfile(filename.c_str(), ios::out | ios::app );

    reportfile<<m_Config.projections.sProjectionMask<<"\t"
			<<m_Config.estimator.m_sModule<<"\t"
			<<m_Config.projections.fPeriods<<"\t"
			<<m_Config.projections.nPhaseSteps<<"\t"
			<<m_ProcCoefficients["snr"].substr(0,m_ProcCoefficients["snr"].find(' '))<<"\t"
			<<m_Estimator->GetModule()->ComputeVisibility()*100.0f<<std::endl;

    reportfile.close();

    ostringstream fname;

    fname<<m_Config.projections.sDestPath<<basename<<"_config.xml";
    logger(kipl::logging::Logger::LogMessage,fname.str());
    ofstream conffile(fname.str().c_str(), ios::out | ios::app );

    conffile<<m_Config.WriteXML();

    conffile.close();
}
