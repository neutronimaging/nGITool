/***************************************************************************
 *   Copyright (C) 2005 by Anders Kaestner   *
 *   anders.kaestner@env.ethz.ch   *
 ***************************************************************************/
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <string>
#include <vector>
#include <ConfigBase.h>

/**
@author Anders Kaestner
*/
class RootTrackConfig : public ConfigBase
{
	public:
        RootTrackConfig();
        virtual std::string WriteXML();
		int help();
        struct DLL_EXPORT cImageInformation {
            cImageInformation();
            cImageInformation(const cImageInformation &a);
            cImageInformation & operator=(const cImageInformation &a);
            std::string WriteXML(size_t indent=0);
            void ParseXML(xmlTextReaderPtr reader);

            std::string filename;
            std::string path;
            std::string basename;
            std::vector<std::string> extensions;
            std::string maskname;
            bool useroi;
            size_t img_roi[4];
            int img_rot;
            int img_log;
        };

        struct DLL_EXPORT cProcessingSettings {
            cProcessingSettings();
            cProcessingSettings(const cProcessingSettings &a);
            cProcessingSettings & operator=(const cProcessingSettings &a);
            std::string WriteXML(size_t indent=0);
            void ParseXML(xmlTextReaderPtr reader);

            int m_nSmallItem;
            float thres_quantile;
            int thres_clean;
            float thres_low;
            size_t m_nHistogramSize;
            float mfr_sigma;
            int mfr_dirs;
            float mfr_ratio;
            float mfr_omega;
            bool m_bTracking;
            int piece_maxsize;
            int piece_minsize;
            int piece_testsize;
            float grow_thresmin;
            float grow_thresdec;
            float piece_tree;
            float piece_fringe;
            float sample_thick;
            float sample_volume;
            bool save_steps;
            int vis_seginfo;
            int vol_est;
            float pixel_size;
            std::string pixel_unit;
        };

        cImageInformation mImageInformation;
        cProcessingSettings mProcessingSettings;

protected:
        virtual void ParseConfig(xmlTextReaderPtr reader, std::string sName);
        virtual void ParseProcessChain(xmlTextReaderPtr reader);
};

#endif
