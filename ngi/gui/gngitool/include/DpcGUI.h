/*
 * DpcGUI.h
 *
 *  Created on: Mar 13, 2010
 *      Author: anders
 */

#ifndef DPCGUI_H_
#define DPCGUI_H_

#include <string>
#include <gtkmm.h>
#include <ImageViewermm.h>
#include <PlotViewermm.h>
#include <LogViewermm.h>
#include <ModuleManagerWidget2.h>
#include <ApplicationBase.h>
#include "../include/PreProcModuleConfigurator.h"


#include <logging/logger.h>

#include <nGIConfig.h>
#include <nGIProcessor.h>
#include <nGIFactory.h>
#include <nGIEngine.h>

class DpcGUI : public ApplicationBase {
	kipl::logging::Logger logger;
public:
	DpcGUI(Glib::RefPtr<Gtk::Builder> refxml, std::string application_path);
	virtual ~DpcGUI();
	
	nGIConfig mConfig;
	void UpdateDialog();
	void UpdateConfig();

	// Call-backs
	void CB_menu_about ();
	void CB_UseROI_Changed();
	void CB_UseNormROI_Changed();
	void CB_Preview_Clicked();
	void CB_ProcessAll_Clicked();
	void CB_VisibilityWindow_Changed();
	void CB_menu_new ();
	void CB_menu_load ();
	void CB_menu_save ();
	void CB_menu_save_as ();
	void CB_menu_quit ();
	void CB_menu_createreport();
	void CB_button_DpathRef();
	void CB_button_DpathDark();
	void CB_button_DpathProc();
	void CB_ClampAmpl_Changed();
	void CB_ClampDFI_Changed();


	// Modifiers
	void ToggleROIEntries();
	void ToggleNormROIEntries();
	void ToggleClampAmplitudeEntries();
	void ToggleClampDFIEntries();

	void on_changed_roi();
	void on_changed_normroi();

	// Processing
	void ProcessAll();
	void Preview();
	void Process(nGIConfig &config);

	// Menu stuff
	void Menu_new ();
	void Menu_load ();
	void Menu_save ();
	void Menu_save_as ();
	
	Gtk::Window & get_window() const {return *main_Window;}

protected:
//	SpotCleanDialog spotdlg;
	void SaveResult(kipl::base::TImage<float,2> & img, std::string mask, int index);
	void DisplayNewProjection();
	void ShowResults();
	void Draw_VisibilityWindow();
	void LoadDefaults();

	std::string sCurrentFileName;
	std::string sApplicationPath;
//	DpcProcessor processor;

	Gtk::Window        * main_Window;
	Gtk::AboutDialog   * about_dialog;
	Gtk::ImageMenuItem * menu_New;
	Gtk::ImageMenuItem * menu_Open;
	Gtk::ImageMenuItem * menu_Save;
	Gtk::ImageMenuItem * menu_SaveAs;
	Gtk::ImageMenuItem * menu_Quit;
	Gtk::ImageMenuItem * menu_About;
	Gtk::ImageMenuItem * menu_CreateReport;

	Gtk::Entry * entry_project;
	Gtk::Entry * entry_operator;
	Gtk::Entry * entry_instrument;
	Gtk::Entry * entry_sample;
	Gtk::Button * btn_Preview;
	Gtk::Button * btn_ProcessAll;
	Gtk::Button * btn_CreateReport;
	Gtk::Frame  * frame_preview;
	Gtk::Alignment * align_preview;
	Gtk::Alignment * align_oscillation_plot;
	Gtk::Alignment * align_visibility;
	Gtk::Alignment * align_projections;
	Gtk::Alignment * align_amplitude_image;
	Gtk::Alignment * align_DPC_image;
	Gtk::Alignment * align_DFI_image;
	Gtk::Alignment * align_logger;
	Gtk::Alignment * align_preprocmodules;
	Gtk::Alignment * align_estmodule;

	Gtk::Scale * scale_projections;

	Gtk::FileChooserButton * fcb_phasefiles;
	Gtk::FileChooserButton * fcb_references;
	Gtk::FileChooserButton * fcb_darkcurrent;

	Gtk::Button * button_DpathRef;
	Gtk::Button * button_DpathDark;
	Gtk::Button * button_DpathProc;
	Gtk::Entry * entry_PhaseMask;
	Gtk::Entry * entry_OBMask;
	Gtk::Entry * entry_DCMask;
	Gtk::Entry * entry_DestMask;

	Gtk::SpinButton * sb_FilesCnt;
	Gtk::SpinButton * sb_FileStride;
	Gtk::SpinButton * sb_DCCnt;
	Gtk::SpinButton * sb_FirstIndex;
	Gtk::SpinButton * sb_DarkFirstIndex;
	Gtk::SpinButton * sb_RefFirstIndex;
	Gtk::SpinButton * sb_RefFileStride;
	Gtk::FileChooserButton *fcb_destination;
	Gtk::SpinButton  * sb_Samples;
	Gtk::SpinButton  * sb_Periods;
	Gtk::CheckButton * cb_CompletePeriod;
	Gtk::SpinButton  * sb_ROIx1;
	Gtk::SpinButton  * sb_ROIy1;
	Gtk::SpinButton  * sb_ROIx2;
	Gtk::SpinButton  * sb_ROIy2;
	Gtk::SpinButton  * sb_nROIx1;
	Gtk::SpinButton  * sb_nROIy1;
	Gtk::SpinButton  * sb_nROIx2;
	Gtk::SpinButton  * sb_nROIy2;
	Gtk::CheckButton * cb_UseROI;
	Gtk::CheckButton * cb_UseNROI;
	Gtk::CheckButton * cb_PreviewCropped;
	Gtk::CheckButton * cb_amplitude;
	Gtk::CheckButton * cb_dpc;
	Gtk::CheckButton * cb_dfi;
	Gtk::CheckButton * cb_vismap;
	Gtk::Label       * label_visibility;
	Gtk::ProgressBar * progressbar;

	Gtk::TextView	 * tw_comment;
	Gtk::SpinButton  * sb_visibilitywindow;
	Gtk::SpinButton  * sb_window_posx;
	Gtk::SpinButton  * sb_window_posy;

	Gtk::CheckButton * cb_clamp_dfi;
	Gtk::SpinButton * sb_clamp_dfi_lo;
	Gtk::SpinButton * sb_clamp_dfi_hi;

	Gtk::CheckButton * cb_clamp_ampl;

	Gtk::SpinButton * sb_clamp_ampl_lo;
	Gtk::SpinButton * sb_clamp_ampl_hi;

	Gtk_addons::PlotViewer viewer_plot;
	Gtk_addons::ImageViewer viewer_visibility;
	Gtk_addons::ImageViewer viewer_projections;
	Gtk_addons::ImageViewer viewer_amplitude;
	Gtk_addons::ImageViewer viewer_DPC;
	Gtk_addons::ImageViewer viewer_DFI;
	Gtk_addons::LogViewer log_viewer;

	PreProcModuleConfigurator module_configurator;
	ModuleManagerWidget2 module_manager;
	nGIFactory m_Factory;
	nGIEngine *m_pEngine;
	float m_fVisibility;

};

#endif /* DPCGUI_H_ */
