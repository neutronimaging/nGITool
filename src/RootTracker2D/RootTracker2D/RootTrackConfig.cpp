// ***************************************************************************
//    Copyright (C) 2010 by Anders Kaestner   
//    anders.kaestner@psi.ch   
//                                                                          
// ***************************************************************************
#include "RootTrackConfig.h"

#include <iostream>
#include <strings/parenc.h>
#include <strings/filenames.h>
#include <cstring>
#include <sstream>
#include <ModuleException.h>
#include <strings/miscstring.h>
#include <strings/string2array.h>

using namespace std;

RootTrackConfig::RootTrackConfig() :
    ConfigBase("roottracker")
{	
}

std::string RootTrackConfig::WriteXML()
{
    std::ostringstream str;

    int indent=4;
    str<<"<"<< ConfigBase::m_sName <<">\n";
        str<<UserInformation.WriteXML(indent);
        str<<mImageInformation.WriteXML(indent);
        str<<mProcessingSettings.WriteXML(indent);
    str<<"</"<< ConfigBase::m_sName <<">"<<std::endl;
    return str.str();
}

int RootTrackConfig::help()
{
	cout<<"filename       filename to process"<<endl;
	cout<<"img_crop       cropping region of the image x1 y1 x2 y2"<<endl;
	cout<<"img_rot        rotation of the image N.90"<<endl;
	cout<<"small_item     threshold for small skeleton segments"<<endl;
	cout<<"thres_bin      threshold in percent of black pixels"<<endl; 
	cout<<"thres_low      threshold in percent of all pixels to limit the dynamics"<<endl;
	cout<<"hist_size      number of histogram bins"<<endl;
	cout<<"mfr_sigma      width of the Gaussian kernel"<<endl;
	cout<<"mfr_dirs       number of filter orientations"<<endl;
	cout<<"mfr_ratio      width ratio of the kernel"<<endl;
	cout<<"do_tracking    activate the tracking algorithm"<<endl;
	cout<<"piece_maxsize  upper limit of a segmented piece"<<endl;
	cout<<"piece_minsize  lower limit of a segmented piece"<<endl;
	cout<<"piece_testsize a piece must reach this size to be tested"<<endl;
	cout<<"piece_tree     number of crossings/skeletonlength "<<endl;
	cout<<"piece_fringe   number of edge pixels in contact with another segment"<<endl;
	cout<<"grow_thresmin  minimum threshold to use during probing"<<endl;
	cout<<"grow_thresdec  threshold decrement value during probing"<<endl;
	cout<<"quiet          reduce the screen output"<<endl;
	cout<<"vis_blind      switch off the iteration visualization"<<endl;
	cout<<"vis_seginfo    show to label the segments"<<endl;
	cout<<"vis_hist       show histogram of mfr image"<<endl;
	cout<<"save_steps     save the result of all processing steps"<<endl;
	cout<<"vol_est        compute an estimate of the root volume regions can be specified with parameter maskname"<<endl;
	cout<<"maskname       filename of image with labelled regions"<<endl;
	cout<<"pixel_size     length of one side of the pixel"<<endl;
	cout<<"pixel_unit     string containing the unit"<<endl;
	cout<<"sample_thick   thickness of the smaple container, used to compute root volume fraction"<<endl;
	cout<<"sample_volume  volume of the sample, alternative to sample_thick. Use this if you know the volume"<<endl;
	return 1;
}

void RootTrackConfig::ParseConfig(xmlTextReaderPtr reader, std::string sName)
{

    if (sName=="imageinfo") {
        mImageInformation.ParseXML(reader);
    }

    if (sName=="settings") {
        mProcessingSettings.ParseXML(reader);
    }
}

void RootTrackConfig::ParseProcessChain(xmlTextReaderPtr reader)
{
    logger(kipl::logging::Logger::LogVerbose,"There is no process chain implemented in this configuration. Step skipped.");
}


RootTrackConfig::cImageInformation::cImageInformation() :
    filename("noname.tif"),
    maskname(""),
    useroi(false),
    img_rot(0),
    img_log(1)
{
    img_roi[0]=0;
    img_roi[1]=0;
    img_roi[2]=100;
    img_roi[3]=100;
}

RootTrackConfig::cImageInformation::cImageInformation(const RootTrackConfig::cImageInformation &a) :
filename(a.filename),
maskname(a.maskname),
useroi(a.useroi),
img_rot(a.img_rot),
img_log(a.img_log)
{
    img_roi[0]=a.img_roi[0];
    img_roi[1]=a.img_roi[1];
    img_roi[2]=a.img_roi[2];
    img_roi[3]=a.img_roi[3];
}

RootTrackConfig::cImageInformation & RootTrackConfig::cImageInformation::operator=(const RootTrackConfig::cImageInformation &a)
{
    filename = a.filename;
    maskname = a.maskname;
    useroi   = a.useroi;
    img_rot  = a.img_rot;
    img_log  = a.img_log;

    img_roi[0]=a.img_roi[0];
    img_roi[1]=a.img_roi[1];
    img_roi[2]=a.img_roi[2];
    img_roi[3]=a.img_roi[3];

    return *this;
}

std::string RootTrackConfig::cImageInformation::WriteXML(size_t indent)
{
    std::ostringstream str;

    str<<setw(indent)  <<" "<<"<imageinfo>"<<std::endl;
    str<<setw(indent+4)  <<" "<<"<filename>"<<filename<<"</filename>"<<endl;
    str<<setw(indent+4)  <<" "<<"<use_roi>"<<(useroi ? "true": "false")<<"</use_roi>"<<endl;
    str<<setw(indent+4)  <<" "<<"<img_roi>"<<img_roi[0]<<" "<<img_roi[1]<<" "<<img_roi[2]<<" "<<img_roi[3]<<"</img_roi>"<<endl;
    str<<setw(indent+4)  <<" "<<"<img_rot>"<<img_rot<<"</img_rot>"<<endl;
    str<<setw(indent+4)  <<" "<<"<img_log>"<<img_log<<"</img_log>"<<endl;
    str<<setw(indent+4)<<" "<<"<maskname>"<<maskname<<"</maskname>"<<endl;
    str<<setw(indent)  <<" "<<"</imageinfo>"<<std::endl;

    return str.str();
}

void RootTrackConfig::cImageInformation::ParseXML(xmlTextReaderPtr reader)
{
    cout<<"parsing imageinfo"<<endl;
    const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1) {
        if (xmlTextReaderNodeType(reader)==1) {
            name = xmlTextReaderConstName(reader);
            ret=xmlTextReaderRead(reader);

            value = xmlTextReaderConstValue(reader);
            if (name==NULL) {
                throw ModuleException("Unexpected image information contents in parameter file",__FILE__,__LINE__);
            }
            if (value!=NULL)
                sValue=reinterpret_cast<const char *>(value);
            else
                sValue="Empty";
            sName=reinterpret_cast<const char *>(name);

            cout<<sName<<"="<<sValue<<endl;
            if (sName=="filename") {
                filename=sValue;
            }

            if (sName=="use_roi") {
                useroi=kipl::strings::string2bool(sValue);
            }

            if (sName=="img_roi") {
                kipl::strings::String2Array(sValue,img_roi,4);
            }

            if (sName=="img_rot") {
                img_rot=atoi(sValue.c_str());
            }

            if (sName=="img_log") {
                img_log=atoi(sValue.c_str());
            }

            if (sName=="maskname") {
                maskname=sValue;
            }

        }
        ret = xmlTextReaderRead(reader);
        if (xmlTextReaderDepth(reader)<depth)
            ret=0;
    }
}


RootTrackConfig::cProcessingSettings::cProcessingSettings() :
    m_nSmallItem(7),
    thres_quantile(0.90f),
    thres_clean(0),
    thres_low(0.2f),
    m_nHistogramSize(1024),
    mfr_sigma(1.5f),
    mfr_dirs(12),
    mfr_ratio(0.1f),
    mfr_omega(0.0f),
    m_bTracking(false),
    piece_maxsize(1000),
    piece_minsize(100),
    piece_testsize(30),
    grow_thresmin(0.06f),
    grow_thresdec(0.01f),
    piece_tree(0.04f),
    piece_fringe(0.05f),
    sample_thick(1.0f),
    sample_volume(1.0f),
    save_steps(0),
    vol_est(0),
    pixel_size(0.27f),
    pixel_unit("mm")
{

}

RootTrackConfig::cProcessingSettings::cProcessingSettings(const cProcessingSettings &a) :
    m_nSmallItem(a.m_nSmallItem),
    thres_quantile(a.thres_quantile),
    thres_clean(a.thres_clean),
    thres_low(a.thres_low),
    m_nHistogramSize(a.m_nHistogramSize),
    mfr_sigma(a.mfr_sigma),
    mfr_dirs(a.mfr_dirs),
    mfr_ratio(a.mfr_ratio),
    mfr_omega(a.mfr_omega),
    m_bTracking(a.m_bTracking),
    piece_maxsize(a.piece_maxsize),
    piece_minsize(a.piece_minsize),
    piece_testsize(a.piece_testsize),
    grow_thresmin(a.grow_thresmin),
    grow_thresdec(a.grow_thresdec),
    piece_tree(a.piece_tree),
    piece_fringe(a.piece_fringe),
    sample_thick(a.sample_thick),
    sample_volume(a.sample_volume),
    save_steps(a.save_steps),
    vol_est(a.vol_est),
    pixel_size(a.pixel_size),
    pixel_unit(a.pixel_unit)
{

}

RootTrackConfig::cProcessingSettings & RootTrackConfig::cProcessingSettings::operator=(const RootTrackConfig::cProcessingSettings &a)
{
    m_nSmallItem     = a.m_nSmallItem;
    thres_quantile   = a.thres_quantile;
    thres_clean      = a.thres_clean;
    thres_low        = a.thres_low;
    m_nHistogramSize = a.m_nHistogramSize;
    mfr_sigma        = a.mfr_sigma;
    mfr_dirs         = a.mfr_dirs;
    mfr_ratio        = a.mfr_ratio;
    mfr_omega        = a.mfr_sigma;
    m_bTracking      = a.m_bTracking;
    piece_maxsize    = a.piece_maxsize;
    piece_minsize    = a.piece_minsize;
    piece_testsize   = a.piece_testsize;
    grow_thresmin    = a.grow_thresmin;
    grow_thresdec    = a.grow_thresdec;
    piece_tree       = a.piece_tree;
    piece_fringe     = a.piece_fringe;
    sample_thick     = a.sample_thick;
    sample_volume    = a.sample_volume;
    save_steps       = a.save_steps;
    vol_est          = a.vol_est;
    pixel_size       = a.pixel_size;
    pixel_unit       = a.pixel_unit;

    return *this;
}

std::string RootTrackConfig::cProcessingSettings::WriteXML(size_t indent)
{
    std::ostringstream str;

    str<<setw(indent)  <<" "<<"<settings>"<<std::endl;
    str<<setw(indent+4)<<" "<<"<small_item>"<<m_nSmallItem<<"</small_item>"<<endl;
    str<<setw(indent+4)<<" "<<"<thres_quantile>"<<thres_quantile<<"</thres_quantile>"<<endl;
    str<<setw(indent+4)<<" "<<"<thres_low>"<<thres_low<<"</thres_low>"<<endl;
    str<<setw(indent+4)<<" "<<"<hist_size>"<<m_nHistogramSize<<"</hist_size>"<<endl;
    str<<setw(indent+4)<<" "<<"<mfr_sigma>"<<mfr_sigma<<"</mfr_sigma>"<<endl;
    str<<setw(indent+4)<<" "<<"<mfr_dirs>"<<mfr_dirs<<"</mfr_dirs>"<<endl;
    str<<setw(indent+4)<<" "<<"<mfr_ratio>"<<mfr_ratio<<"</mfr_ratio>"<<endl;
    str<<setw(indent+4)<<" "<<"<mfr_omega>"<<mfr_omega<<"</mfr_omega>"<<endl;
    str<<setw(indent+4)<<" "<<"<do_tracking>"<<(m_bTracking ? "true":"false")<<"</do_tracking>"<<endl;
    str<<setw(indent+4)<<" "<<"<piece_maxsize>"<<piece_maxsize<<"</piece_maxsize>"<<endl;
    str<<setw(indent+4)<<" "<<"<piece_minsize>"<<piece_minsize<<"</piece_minsize>"<<endl;
    str<<setw(indent+4)<<" "<<"<piece_testsize>"<<piece_testsize<<"</piece_testsize>"<<endl;
    str<<setw(indent+4)<<" "<<"<piece_tree>"<<piece_tree<<"</piece_tree>"<<endl;
    str<<setw(indent+4)<<" "<<"<piece_fringe>"<<piece_fringe<<"</piece_fringe>"<<endl;
    str<<setw(indent+4)<<" "<<"<grow_thresmin>"<<grow_thresmin<<"</grow_thresmin>"<<endl;
    str<<setw(indent+4)<<" "<<"<grow_thresdec>"<<grow_thresdec<<"</grow_thresdec>"<<endl;

    str<<setw(indent+4)<<" "<<"<save_steps>"<<(save_steps ? "true":"false")<<"</save_steps>"<<endl;
    str<<setw(indent+4)<<" "<<"<vol_est>"<<vol_est<<"</vol_est>"<<endl;
    str<<setw(indent+4)<<" "<<"<pixel_size>"<<pixel_size<<"</pixel_size>"<<endl;
    str<<setw(indent+4)<<" "<<"<pixel_unit>"<<pixel_unit<<"</pixel_unit>"<<endl;
    str<<setw(indent+4)<<" "<<"<sample_thick>"<<sample_thick<<"</sample_thick>"<<endl;
    str<<setw(indent+4)<<" "<<"<sample_volume>"<<sample_volume<<"</sample_volume>"<<endl;
    str<<setw(indent)  <<" "<<"</settings>"<<std::endl;

    return str.str();
}

void RootTrackConfig::cProcessingSettings::ParseXML(xmlTextReaderPtr reader)
{
    cout<<"parsing settings"<<endl;
    const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1) {
        if (xmlTextReaderNodeType(reader)==1) {
            name = xmlTextReaderConstName(reader);
            ret=xmlTextReaderRead(reader);

            value = xmlTextReaderConstValue(reader);
            if (name==NULL) {
                throw ModuleException("Unexpected processing settings in parameter file",__FILE__,__LINE__);
            }
            if (value!=NULL)
                sValue=reinterpret_cast<const char *>(value);
            else
                sValue="Empty";
            sName=reinterpret_cast<const char *>(name);

            if (sName=="small_item") {
                m_nSmallItem = atoi(sValue.c_str());
            }
            if (sName=="thres_quantile") {
                thres_quantile = atof(sValue.c_str());
            }

            if (sName=="thres_low") {
                thres_low = atof(sValue.c_str());
            }

            if (sName=="hist_size") {
                m_nHistogramSize = atoi(sValue.c_str());
            }

            if (sName=="mfr_sigma") {
                mfr_sigma = atof(sValue.c_str());
            }

            if (sName=="mfr_dirs") {
                mfr_dirs = atoi(sValue.c_str());
            }

            if (sName=="mfr_ratio") {
                mfr_ratio = atof(sValue.c_str());
            }

            if (sName=="mfr_omega") {
                mfr_omega = atof(sValue.c_str());
            }

            if (sName=="do_tracking") {
                m_bTracking=kipl::strings::string2bool(sValue);;
            }

            if (sName=="piece_maxsize") {
                piece_maxsize = atof(sValue.c_str());
            }

            if (sName=="piece_minsize") {
                piece_minsize = atof(sValue.c_str());
            }

            if (sName=="piece_testsize") {
                piece_testsize = atoi(sValue.c_str());
            }

            if (sName=="piece_tree") {
                piece_tree = atof(sValue.c_str());
            }

            if (sName=="piece_fringe") {
                piece_fringe = atof(sValue.c_str());
            }

            if (sName=="grow_thresmin") {
                grow_thresmin = atof(sValue.c_str());
            }

            if (sName=="grow_thresdec") {
                grow_thresdec = atof(sValue.c_str());
            }

            if (sName=="save_steps") {
                save_steps=kipl::strings::string2bool(sValue);;
            }


            if (sName=="vol_est") {
                vol_est = atoi(sValue.c_str());
            }

            if (sName=="pixel_unit") {
                pixel_unit = sValue;
            }


            if (sName=="pixel_size") {
                pixel_size = atof(sValue.c_str());
            }

            if (sName=="sample_thick") {
                sample_thick=atof(sValue.c_str());
            }

            if (sName=="sample_volume") {
                sample_volume=atof(sValue.c_str());
            }
        }
        ret = xmlTextReaderRead(reader);
        if (xmlTextReaderDepth(reader)<depth)
            ret=0;
    }
}
