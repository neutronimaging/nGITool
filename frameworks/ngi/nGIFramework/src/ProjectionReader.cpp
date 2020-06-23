//<LICENSE>

#include "stdafx.h"

#include <map>
#include <string>
#include <iostream>
#include <base/timage.h>
#include <io/io_tiff.h>
#include <io/io_fits.h>
#include <math/median.h>
#include "../include/nGIException.h"
#include "../include/ProjectionReader.h"
//#include "../include/ReconHelpers.h"

ProjectionReader::ProjectionReader(InteractionBase *interactor) : 
	logger("ProjectionReader"),
		m_Interactor(interactor)
{

}

ProjectionReader::~ProjectionReader(void)
{

}

std::vector<size_t> ProjectionReader::GetImageSize(const string &path,
                                   const string &filemask,
                                   size_t number)
{
	std::string filename;
	std::string ext;
	kipl::strings::filenames::MakeFileName(path+filemask,number,filename,ext,'#','0');

    return GetImageSize(filename);
}

std::vector<size_t> ProjectionReader::GetImageSize(std::string filename)
{
//	throw nGIException("GetImageSize is not implemented",__FILE__,__LINE__);

	std::map<std::string, size_t> extensions;
	extensions[".mat"]=0;
	extensions[".fits"]=1;
	extensions[".fit"]=1;
	extensions[".fts"]=1;
	extensions[".tif"]=2;
	extensions[".tiff"]=2;
	extensions[".TIF"]=2;
	extensions[".png"]=3;

	size_t extpos=filename.find_last_of(".");
	std::stringstream msg;
    std::vector<size_t> dims;
	try {
		if (extpos!=filename.npos) {
			std::string ext=filename.substr(extpos);
			switch (extensions[ext]) {
            case 0  : throw nGIException("Unknown file type",__FILE__, __LINE__); break;
            case 1  : dims = kipl::io::GetFITSDims(filename); break;
            case 2  : dims = kipl::io::GetTIFFDims(filename);  break;
			//case 3  : return GetImageSizePNG(filename.c_str(),dims);  break;

			default : throw nGIException("Unknown file type",__FILE__, __LINE__); break;
			}
		}
		else {
			throw nGIException("Unknown file type",__FILE__, __LINE__);
		}
	}
	catch (std::exception &e) {
		throw nGIException(e.what(),__FILE__,__LINE__);
	}
	catch (kipl::base::KiplException &e) {
		throw nGIException(e.what(),__FILE__,__LINE__);
	}

}

kipl::base::TImage<float,2> ProjectionReader::ReadFITS(std::string filename, const std::vector<size_t> & nCrop)
{
	kipl::base::TImage<float,2> img;
	try {
        kipl::io::ReadFITS(img,filename,nCrop);
	}
	catch (std::exception &e) {
		throw nGIException(e.what(), __FILE__,__LINE__);
	}
	catch (kipl::base::KiplException &e) {
		throw kipl::base::KiplException(e.what(), __FILE__,__LINE__);
	}
	catch (...) {
		throw nGIException("Unknown exception", __FILE__,__LINE__);
	}
	
	return img;
}

kipl::base::TImage<float,2> ProjectionReader::ReadTIFF(std::string filename, const std::vector<size_t> &nCrop)
{
	kipl::base::TImage<float,2> img;
    kipl::io::ReadTIFF(img,filename,nCrop,0UL);
	
	return img;
}

kipl::base::TImage<float,2> ProjectionReader::ReadPNG(std::string filename, const std::vector<size_t> & nCrop)
{
	throw nGIException("ReadPNG is not implemented",__FILE__, __LINE__);
	return kipl::base::TImage<float,2>();
}

pair<float,float> ProjectionReader::GetProjectionDose(std::string filename, const std::vector<size_t> & nDoseROI)
{
	kipl::base::TImage<float,2> img;

	if (!(nDoseROI[0]*nDoseROI[1]*nDoseROI[2]*nDoseROI[3]))
		return make_pair<float,float>(1.0f,1.0f);

	img=Read(filename,nDoseROI);

	float *pImg=img.GetDataPtr();

	float *sum  = new float[img.Size(1)];
	float *sum2 = new float[img.Size(1)];
	memset(sum,0,img.Size(1)*sizeof(float));
	memset(sum2,0,img.Size(1)*sizeof(float));

	for (size_t y=0; y<img.Size(1); y++) {
		pImg=img.GetLinePtr(y);
		
		for (size_t x=0; x<img.Size(0); x++) {
			sum[y]+=pImg[x];
			sum2[y]+=pImg[x]*pImg[x];
		}
	}

	float s=0.0f, s2=0.0f;
	kipl::math::median(sum,img.Size(1),&s);
	kipl::math::median(sum2,img.Size(1),&s2);

	float N=static_cast<float>(img.Size(0));

	pair<float,float> dose(s/N,sqrt(1/(N*N-1)*(s2-s*s/(N*N))));
	dose.second=dose.first/dose.second;

	delete [] sum;
	delete [] sum2;

	return dose;
}

float ProjectionReader::GetProjectionDose(std::string path, std::string filemask, size_t number, std::vector<size_t> &nDoseROI)
{
	std::string filename;
	std::string ext;
	kipl::strings::filenames::MakeFileName(path+filemask,number,filename,ext,'#','0');

	pair<float,float> dose=GetProjectionDose(filename,nDoseROI);
	return dose.first;
}

kipl::base::TImage<float,3> ProjectionReader::Read(nGIConfig config,std::string filemask, size_t firstindex, size_t stride, size_t index, std::map<std::string, std::string>  &parameters)
{
	kipl::base::TImage<float,2> proj;

    std::vector<size_t> dims(3,0UL);

	kipl::base::TImage<float,3> img;

	std::ostringstream dose;
	std::ostringstream SNR;
	std::string filename;
	std::string ext;
	pair<float, float> dose_reading;

	for (size_t i=0; i<config.projections.nPhaseSteps ; i++) {
		kipl::strings::filenames::MakeFileName(filemask,
				firstindex+index+i*stride,
				filename,ext,
				'#','0');

		std::cout<<filename<<std::endl;
		dose_reading=GetProjectionDose(filename,config.projections.nNormROI);
		dose   << dose_reading.first<<" ";
		SNR   << dose_reading.second<<" ";

		if (config.projections.bUseROI) {
			proj = Read(filename,config.projections.nROI);
		}
		else {
            proj = Read(filename,{});
		}
		if (i==0) {
			dims[0]=proj.Size(0);
			dims[1]=proj.Size(1);
			dims[2]=config.projections.nPhaseSteps;
            img.resize(dims);
		}
		float *pProj=proj.GetDataPtr();
		float *pImg=img.GetLinePtr(0,i);
		memcpy(pImg,pProj,sizeof(float)*proj.Size());

	}
	parameters["dose"]=dose.str();
	parameters["snr"]=SNR.str();
	
	return img;
}

kipl::base::TImage<float,2> ProjectionReader::Read(std::string filename, const std::vector<size_t> & nCrop)
{
	std::map<std::string, size_t> extensions;
	extensions[".mat"]=0;
	extensions[".fits"]=1;
	extensions[".fit"]=1;
	extensions[".fts"]=1;
	extensions[".tif"]=2;
	extensions[".tiff"]=2;
	extensions[".TIF"]=2;
	extensions[".png"]=3;

	size_t extpos=filename.find_last_of(".");
	std::stringstream msg;

	if (extpos!=filename.npos) {
		std::string ext=filename.substr(extpos);
		switch (extensions[ext]) {
        case 0  : throw nGIException("Unknown file type",__FILE__, __LINE__); break;
		case 1  : return ReadFITS(filename,nCrop); break;
		case 2  : return ReadTIFF(filename,nCrop);  break;
		case 3  : return ReadPNG(filename,nCrop);  break;
		//case 4	: return ReadHDF(filename);  break;
		default : throw nGIException("Unknown file type",__FILE__, __LINE__); break;
		}
	}
	else {
		throw nGIException("Unknown file type",__FILE__, __LINE__);
	}

	return kipl::base::TImage<float,2>();
}

bool ProjectionReader::UpdateStatus(float val, std::string msg)
{
	if (m_Interactor!=NULL)
		return m_Interactor->SetProgress(val, msg);;

	return false;
}

bool ProjectionReader::Aborted()
{
	if (m_Interactor!=NULL)
		return m_Interactor->Aborted();

	return false;
}
