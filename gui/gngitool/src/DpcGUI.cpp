/*
 * DpcGUI.cpp
 *
 *  Created on: Mar 13, 2010
 *      Author: anders
 */
#include "stdafx.h"
#include "../include/DpcGUI.h"
#include <nGIException.h>
#include "../include/nGIReportGenerator.h"
#include <nGIFactory.h>
#include <nGIEngine.h>

#include <gtkmm.h>
#include <exception>
#include <fstream>


#include <ModuleException.h>
#include <base/KiplException.h>
#include <strings/filenames.h>
#include <io/io_matlab.h>
#include <io/io_tiff.h>
#include <base/thistogram.h>

DpcGUI::DpcGUI(Glib::RefPtr<Gtk::Builder> refxml, std::string ApplicationPath) :
logger("DpcGUI"),
sApplicationPath(ApplicationPath),
main_Window(NULL),
module_configurator(&mConfig),
module_manager("ngitool",ApplicationPath,&module_configurator),
m_pEngine(NULL),
m_fVisibility(0.0)
{
	logger(kipl::logging::Logger::LogVerbose,"Enter GUI constructor");
	refxml->get_widget("DPC_wnd_main",main_Window);
	refxml->get_widget("DPC_dlg_about",about_dialog);
	refxml->get_widget("DPC_menu_New",menu_New);
	refxml->get_widget("DPC_menu_Open",menu_Open);
	refxml->get_widget("DPC_menu_Save",menu_Save);
	refxml->get_widget("DPC_menu_SaveAs",menu_SaveAs);
	refxml->get_widget("DPC_menu_Quit",menu_Quit);
	refxml->get_widget("DPC_menu_About",menu_About);
	refxml->get_widget("DPC_menu_CreateReport",menu_CreateReport);

	refxml->get_widget("DPC_entry_project",entry_project);
	refxml->get_widget("DPC_entry_operator",entry_operator);
	refxml->get_widget("DPC_entry_instrument",entry_instrument);
	refxml->get_widget("DPC_entry_sample",entry_sample);

	refxml->get_widget("DPC_b_Preview",btn_Preview);
	refxml->get_widget("DPC_b_ProcessAll",btn_ProcessAll);
	refxml->get_widget("DPC_b_CreateReport",btn_CreateReport);
	refxml->get_widget("DPC_align_oscplot",align_oscillation_plot);
	refxml->get_widget("DPC_align_visibility",align_visibility);
	refxml->get_widget("DPC_align_projections",align_projections);
	refxml->get_widget("DPC_align_amplitude",align_amplitude_image);
	refxml->get_widget("DPC_align_DPC",align_DPC_image);
	refxml->get_widget("DPC_align_DFI",align_DFI_image);

	refxml->get_widget("DPC_scale_projections",scale_projections);

	refxml->get_widget("DPC_fcb_phasefiles",fcb_phasefiles);
	refxml->get_widget("DPC_fcb_references",fcb_references);
	refxml->get_widget("DPC_fcb_DarkPath",fcb_darkcurrent);
	refxml->get_widget("DPC_fcb_Destination",fcb_destination);
	refxml->get_widget("DPC_button_DpathRef",button_DpathRef);
	refxml->get_widget("DPC_button_DpathDark",button_DpathDark);
	refxml->get_widget("DPC_button_DpathProc",button_DpathProc);
	refxml->get_widget("DPC_entry_PhaseMask",entry_PhaseMask);
	refxml->get_widget("DPC_entry_OBMask",entry_OBMask);
	refxml->get_widget("DPC_entry_DCMask",entry_DCMask);
	refxml->get_widget("DPC_entry_DestMask",entry_DestMask);
	refxml->get_widget("DPC_sb_FilesCnt",sb_FilesCnt);
	refxml->get_widget("DPC_sb_FileStride",sb_FileStride);
	refxml->get_widget("DPC_sb_DCCnt",sb_DCCnt);
	refxml->get_widget("DPC_sb_DarkFirstIndex",sb_DarkFirstIndex);
	refxml->get_widget("DPC_sb_RefFirstIndex",sb_RefFirstIndex);
	refxml->get_widget("DPC_sb_RefFileStride",sb_RefFileStride);
	refxml->get_widget("DPC_sb_FirstIndex",sb_FirstIndex);

	refxml->get_widget("DPC_sb_Samples",sb_Samples);
	refxml->get_widget("DPC_sb_Periods",sb_Periods);
	refxml->get_widget("DPC_cb_CompletePeriod",cb_CompletePeriod);
	refxml->get_widget("DPC_sb_ROIx1",sb_ROIx1);

	refxml->get_widget("DPC_sb_ROIy1",sb_ROIy1);
	refxml->get_widget("DPC_sb_ROIx2",sb_ROIx2);
	refxml->get_widget("DPC_sb_ROIy2",sb_ROIy2);
	refxml->get_widget("DPC_sb_nROIx1",sb_nROIx1);
	refxml->get_widget("DPC_sb_nROIy1",sb_nROIy1);
	refxml->get_widget("DPC_sb_nROIx2",sb_nROIx2);
	refxml->get_widget("DPC_sb_nROIy2",sb_nROIy2);
	refxml->get_widget("DPC_cb_UseROI",cb_UseROI);
	refxml->get_widget("DPC_cb_PreviewCropped",cb_PreviewCropped);
	refxml->get_widget("DPC_cb_UseNROI",cb_UseNROI);
	refxml->get_widget("DPC_cb_amplitude",cb_amplitude);
	refxml->get_widget("DPC_cb_dpc",cb_dpc);
	refxml->get_widget("DPC_cb_dfi",cb_dfi);
	refxml->get_widget("DPC_cb_vismap",cb_vismap);
	refxml->get_widget("DPC_label_visibility",label_visibility);
	refxml->get_widget("DPC_tw_comment",tw_comment);
	refxml->get_widget("DPC_sb_viswin",sb_visibilitywindow);
	refxml->get_widget("DPC_sb_winposx",sb_window_posx);
	refxml->get_widget("DPC_sb_winposy",sb_window_posy);

	refxml->get_widget("DPC_cb_clamp_dfi",cb_clamp_dfi);
	refxml->get_widget("DPC_sb_clamp_dfi_lo",sb_clamp_dfi_lo);
	refxml->get_widget("DPC_sb_clamp_dfi_hi",sb_clamp_dfi_hi);

	refxml->get_widget("DPC_cb_clamp_ampl",cb_clamp_ampl);
	refxml->get_widget("DPC_sb_clamp_ampl_lo",sb_clamp_ampl_lo);
	refxml->get_widget("DPC_sb_clamp_ampl_hi",sb_clamp_ampl_hi);
	refxml->get_widget("alignment_messages",align_logger);
	refxml->get_widget("alignment_processmodules",align_preprocmodules);
	refxml->get_widget("alignment_estimatormodule",align_estmodule);

	refxml->get_widget("DPC_entry_project",entry_project);
	refxml->get_widget("DPC_entry_operator",entry_operator);
	refxml->get_widget("DPC_entry_instrument",entry_instrument);
	refxml->get_widget("DPC_entry_sample",entry_sample);
	refxml->get_widget("DPC_tw_comment",tw_comment);


    align_logger->add(log_viewer);
    //kipl::logging::Logger::AddLogTarget(log_viewer);
    module_manager.SetApplicationObject(this);
	align_preprocmodules->add(module_manager);
	align_preprocmodules->show_all_children();

	// Setup call-backs
	menu_New->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_new));
	menu_Open->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_load));
	menu_Save->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_save));
	menu_SaveAs->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_save_as));
	menu_About->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_about));
	menu_Quit->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_quit));
	menu_CreateReport->signal_activate().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_createreport));
	btn_CreateReport->signal_clicked().connect(sigc::mem_fun(*this,&DpcGUI::CB_menu_createreport));

	btn_Preview->signal_clicked().connect(sigc::mem_fun(*this,&DpcGUI::CB_Preview_Clicked));

	btn_ProcessAll->signal_clicked().connect(sigc::mem_fun(*this,&DpcGUI::CB_ProcessAll_Clicked));

	button_DpathDark->signal_clicked().connect(sigc::mem_fun(*this,&DpcGUI::CB_button_DpathDark));
	button_DpathRef->signal_clicked().connect(sigc::mem_fun(*this,&DpcGUI::CB_button_DpathRef));
	button_DpathProc->signal_clicked().connect(sigc::mem_fun(*this,&DpcGUI::CB_button_DpathProc));

	cb_UseROI->signal_toggled().connect(sigc::mem_fun(*this,&DpcGUI::CB_UseROI_Changed));
	cb_UseNROI->signal_toggled().connect(sigc::mem_fun(*this,&DpcGUI::CB_UseNormROI_Changed));
	cb_clamp_ampl->signal_toggled().connect(sigc::mem_fun(*this,&DpcGUI::CB_ClampAmpl_Changed));
	cb_clamp_dfi->signal_toggled().connect(sigc::mem_fun(*this,&DpcGUI::CB_ClampDFI_Changed));

	sb_ROIx1->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_roi));
	sb_ROIy1->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_roi));
	sb_ROIx2->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_roi));
	sb_ROIy2->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_roi));

	sb_nROIx1->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_normroi));
	sb_nROIy1->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_normroi));
	sb_nROIx2->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_normroi));
	sb_nROIy2->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::on_changed_normroi));

	sb_visibilitywindow->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::CB_VisibilityWindow_Changed));
	sb_window_posx->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::CB_VisibilityWindow_Changed));
	sb_window_posy->signal_changed().connect(sigc::mem_fun(*this,&DpcGUI::CB_VisibilityWindow_Changed));

	// Setup Viewers
	align_amplitude_image->add(viewer_amplitude);

	size_t dims[2]={256,256};
	kipl::base::TImage<float,2> img(dims);
	img=0.0f;
	viewer_amplitude.set_image(img.GetDataPtr(),img.Dims());
	
	align_DPC_image->add(viewer_DPC);
	viewer_DPC.set_image(img.GetDataPtr(),img.Dims());

	align_DFI_image->add(viewer_DFI);
	viewer_DFI.set_image(img.GetDataPtr(),img.Dims());

	viewer_plot.ShowAxes(true);
	viewer_plot.set_bg_color(Gdk::Color("white"));
	align_oscillation_plot->add(viewer_plot);

	align_visibility->add(viewer_visibility);
	viewer_visibility.set_image(img.GetDataPtr(),img.Dims());

	align_projections->add(viewer_projections);
	viewer_projections.set_image(img.GetDataPtr(),img.Dims());

	align_amplitude_image->show_all_children();
	align_DPC_image->show_all_children();
	align_DFI_image->show_all_children();
	align_oscillation_plot->show_all_children();
	align_visibility->show_all_children();
	align_projections->show_all_children();
	
	std::string path=Glib::get_home_dir();

	mConfig.projections.sDarkPath=path;
	mConfig.projections.sReferencePath=path;
	mConfig.projections.sDestPath=path;
	mConfig.projections.sPath=path;

	scale_projections->signal_value_changed().connect(sigc::mem_fun(*this,
			&DpcGUI::DisplayNewProjection));

	LoadDefaults();
	UpdateDialog();
	ToggleROIEntries();
	ToggleNormROIEntries();
	ToggleClampAmplitudeEntries();
	ToggleClampDFIEntries();

	logger(kipl::logging::Logger::LogVerbose,"Leaving constructor");
}

DpcGUI::~DpcGUI() {
	if (m_pEngine!=NULL)
		delete m_pEngine;
}

void DpcGUI::UpdateDialog()
{
	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sPath,true);
	fcb_phasefiles->set_filename(mConfig.projections.sPath);

	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sReferencePath,true);
	fcb_references->set_filename(mConfig.projections.sReferencePath);

	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sDarkPath,true);
	fcb_darkcurrent->set_filename(mConfig.projections.sDarkPath);
	
	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sDestPath,true);
	fcb_destination->set_filename(mConfig.projections.sDestPath);

	entry_project->set_text(mConfig.UserInformation.sProjectNumber);
	entry_operator->set_text(mConfig.UserInformation.sOperator);
	entry_instrument->set_text(mConfig.UserInformation.sInstrument);
	entry_sample->set_text(mConfig.UserInformation.sSample);
	entry_PhaseMask->set_text(mConfig.projections.sFileMask);
	entry_OBMask->set_text(mConfig.projections.sObMask);
	entry_DCMask->set_text(mConfig.projections.sDarkMask);
	entry_DestMask->set_text(mConfig.projections.sDestMask);

	sb_FilesCnt->set_value(mConfig.projections.nFilesCnt);
	sb_FileStride->set_value(mConfig.projections.nFileStride);
	sb_DCCnt->set_value(mConfig.projections.nDarkCnt);
	sb_FirstIndex->set_value(mConfig.projections.nFirstIndex);
	sb_DarkFirstIndex->set_value(mConfig.projections.nDarkFirstIndex);
	sb_RefFirstIndex->set_value(mConfig.projections.nRefFirstIndex);
	sb_RefFileStride->set_value(mConfig.projections.nRefFileStride);

	sb_Samples->set_value(mConfig.projections.nPhaseSteps);
	sb_Periods->set_value(mConfig.projections.fPeriods);
	cb_CompletePeriod->set_active(mConfig.projections.bCompletePeriod);

	sb_ROIx1->set_value(mConfig.projections.nROI[0]);
	sb_ROIy1->set_value(mConfig.projections.nROI[1]);
	sb_ROIx2->set_value(mConfig.projections.nROI[2]);
	sb_ROIy2->set_value(mConfig.projections.nROI[3]);

	sb_nROIx1->set_value(mConfig.projections.nNormROI[0]);
	sb_nROIy1->set_value(mConfig.projections.nNormROI[1]);
	sb_nROIx2->set_value(mConfig.projections.nNormROI[2]);
	sb_nROIy2->set_value(mConfig.projections.nNormROI[3]);

	cb_UseROI->set_active(mConfig.projections.bUseROI);
	cb_UseNROI->set_active(mConfig.projections.bUseNorm);
	cb_amplitude->set_active(mConfig.process.bComputeAmplitude);
	cb_dpc->set_active(mConfig.process.bComputeDPC);
	cb_dfi->set_active(mConfig.process.bComputeDFI);

	sb_clamp_ampl_lo->set_value(mConfig.process.fAmplLimits[0]);
	sb_clamp_ampl_hi->set_value(mConfig.process.fAmplLimits[1]);

	sb_clamp_dfi_lo->set_value(mConfig.process.fDFILimits[0]);
	sb_clamp_dfi_hi->set_value(mConfig.process.fDFILimits[1]);

	cb_clamp_dfi->set_active(mConfig.process.bUseDFILimits);
	cb_clamp_ampl->set_active(mConfig.process.bUseAmplLimits);

	tw_comment->get_buffer()->set_text(mConfig.UserInformation.sComment);
	sb_visibilitywindow->set_value(mConfig.process.nVisibilityWindow);
	sb_window_posx->set_value(mConfig.process.nVisibilityWindowPos[0]);
	sb_window_posy->set_value(mConfig.process.nVisibilityWindowPos[1]);
	module_manager.SetModules(mConfig.modules);
}

void DpcGUI::UpdateConfig()
{
	mConfig.projections.sPath           = fcb_phasefiles->get_filename();//fcb_phasefiles->get_current_folder();
	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sPath,true);

	mConfig.projections.sReferencePath  = fcb_references->get_filename();//fcb_references->get_current_folder();
	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sReferencePath,true);

	mConfig.projections.sDarkPath       = fcb_darkcurrent->get_filename();//fcb_darkcurrent->get_current_folder();
	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sDarkPath,true);

	mConfig.projections.sDestPath       = fcb_destination->get_filename(); //fcb_destination->get_current_folder();
	kipl::strings::filenames::CheckPathSlashes(mConfig.projections.sDestPath,true);


	mConfig.projections.sFileMask       = entry_PhaseMask->get_text();
	mConfig.projections.sObMask         = entry_OBMask->get_text();
	mConfig.projections.sDarkMask       = entry_DCMask->get_text();
	mConfig.projections.sDestMask       = entry_DestMask->get_text();

	mConfig.projections.nFilesCnt       = sb_FilesCnt->get_value_as_int();
	mConfig.projections.nFileStride		= sb_FileStride->get_value_as_int();
	mConfig.projections.nDarkCnt        = sb_DCCnt->get_value_as_int();
	mConfig.projections.nFirstIndex     = sb_FirstIndex->get_value_as_int();

	mConfig.projections.nDarkFirstIndex = sb_DarkFirstIndex->get_value_as_int();
	mConfig.projections.nRefFirstIndex  = sb_RefFirstIndex->get_value_as_int();
	mConfig.projections.nRefFileStride  = sb_RefFileStride->get_value_as_int();

	mConfig.projections.nPhaseSteps     = sb_Samples->get_value_as_int();
	mConfig.projections.fPeriods        = static_cast<float>(sb_Periods->get_value());
	mConfig.projections.bCompletePeriod = cb_CompletePeriod->get_active();

	mConfig.projections.nROI[0]         = sb_ROIx1->get_value_as_int();
	mConfig.projections.nROI[1]         = sb_ROIy1->get_value_as_int();
	mConfig.projections.nROI[2]         = sb_ROIx2->get_value_as_int();
	mConfig.projections.nROI[3]         = sb_ROIy2->get_value_as_int();

	mConfig.projections.nNormROI[0]     = sb_nROIx1->get_value_as_int();
	mConfig.projections.nNormROI[1]     = sb_nROIy1->get_value_as_int();
	mConfig.projections.nNormROI[2]     = sb_nROIx2->get_value_as_int();
	mConfig.projections.nNormROI[3]     = sb_nROIy2->get_value_as_int();

	mConfig.projections.bUseROI         = cb_UseROI->get_active();
	mConfig.projections.bUseNorm        = cb_UseNROI->get_active();
	mConfig.process.bComputeAmplitude   = cb_amplitude->get_active();
	mConfig.process.bComputeDPC         = cb_dpc->get_active();
	mConfig.process.bComputeDFI         = cb_dfi->get_active();

	mConfig.UserInformation.sComment		= tw_comment->get_buffer()->get_text();
	mConfig.process.nVisibilityWindow       = sb_visibilitywindow->get_value_as_int();
	mConfig.process.nVisibilityWindowPos[0] = sb_window_posx->get_value_as_int();
	mConfig.process.nVisibilityWindowPos[1] = sb_window_posy->get_value_as_int();
	mConfig.process.bUseAmplLimits  = cb_clamp_ampl->get_active();
	mConfig.process.fAmplLimits[0]  = static_cast<float>(sb_clamp_ampl_lo->get_value());
	mConfig.process.fAmplLimits[1]  = static_cast<float>(sb_clamp_ampl_hi->get_value());
	mConfig.process.bUseDFILimits   = cb_clamp_dfi->get_active();
	mConfig.process.fDFILimits[0]   = static_cast<float>(sb_clamp_dfi_lo->get_value());
	mConfig.process.fDFILimits[1]   = static_cast<float>(sb_clamp_dfi_hi->get_value());
	mConfig.modules                 = module_manager.GetModules();

	mConfig.UserInformation.sOperator      = entry_project->get_text();
	mConfig.UserInformation.sInstrument    = entry_operator->get_text();
	mConfig.UserInformation.sProjectNumber = entry_instrument->get_text();
	mConfig.UserInformation.sSample        = entry_sample->get_text();
	mConfig.UserInformation.sComment       = tw_comment->get_buffer()->get_text();
	//mConfig.estimator

}

void DpcGUI::LoadDefaults()
{
	std::ostringstream msg;
	std::string fname = Glib::get_home_dir();

	kipl::strings::filenames::CheckPathSlashes(fname,true);
	fname=fname+"current_ngi.xml";

	try {
		std::ifstream conffile(fname.c_str());
		if (!conffile.good()) {
			kipl::strings::filenames::CheckPathSlashes(sApplicationPath,true);
#ifdef _MSC_VER
			fname=sApplicationPath+"resources\\defaults_windows.xml";
#else
			fname=sApplicationPath+"resources/defaults_linux.xml";
#endif
		}
		else
			conffile.close();

		logger(kipl::logging::Logger::LogVerbose,fname);
		mConfig.LoadConfigFile(fname,"ngi");
	}
	catch (std::exception & e) {
		msg<<"Loading initial settings failed with an STL exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (kipl::base::KiplException & e) {
		msg<<"Loading initial settings failed with an kipl exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	catch (ModuleException & e) {
		msg<<"Loading initial settings failed with an module exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}

	catch (nGIException & e) {
		msg<<"Loading initial settings failed with an nGI exception: "<<e.what();
		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
	catch (...) {
		throw nGIException("An unhandled exception occured during loading initial settings.",__FILE__,__LINE__);

	}
	msg<<"Loaded initial data from "<<fname;
	logger(kipl::logging::Logger::LogMessage,msg.str());

}

// Call-backs

void DpcGUI::CB_menu_new ()
{
	Menu_new();
}
void DpcGUI::CB_menu_load ()
{
	Menu_load();
}

void DpcGUI::CB_menu_save ()
{
	Menu_save();
}

void DpcGUI::CB_menu_save_as ()
{
    Menu_save_as();
}

void DpcGUI::CB_menu_about ()
{
	about_dialog->run();
	about_dialog->hide();
}

void DpcGUI::CB_UseROI_Changed()
{
	mConfig.projections.bUseROI=cb_UseROI->get_active();
	ToggleROIEntries();
}

void DpcGUI::CB_UseNormROI_Changed()
{
	mConfig.projections.bUseNorm=cb_UseNROI->get_active();
	ToggleNormROIEntries();
}

void DpcGUI::CB_ClampAmpl_Changed()
{
	mConfig.process.bUseAmplLimits=cb_clamp_ampl->get_active();
	ToggleClampAmplitudeEntries();
}

void DpcGUI::CB_ClampDFI_Changed()
{
	mConfig.process.bUseDFILimits=cb_clamp_dfi->get_active();
	ToggleClampDFIEntries();
}

void DpcGUI::CB_button_DpathRef()
{
	mConfig.projections.sPath = fcb_phasefiles->get_filename();
	fcb_references->set_filename(mConfig.projections.sPath);
	mConfig.projections.sReferencePath=mConfig.projections.sPath;
}

void DpcGUI::CB_button_DpathDark()
{
	mConfig.projections.sPath = fcb_phasefiles->get_filename();
	fcb_darkcurrent->set_filename(mConfig.projections.sPath);
	mConfig.projections.sDarkPath=mConfig.projections.sPath;
}

void DpcGUI::CB_button_DpathProc()
{
	mConfig.projections.sPath = fcb_phasefiles->get_filename();
	fcb_destination->set_filename(mConfig.projections.sPath);
	mConfig.projections.sDestPath=mConfig.projections.sPath;
}

void DpcGUI::CB_Preview_Clicked()
{
	Preview();
}

void DpcGUI::CB_ProcessAll_Clicked()
{
	ProcessAll();
}

void DpcGUI::CB_VisibilityWindow_Changed()
{
	mConfig.process.nVisibilityWindow = sb_visibilitywindow->get_value_as_int();
	mConfig.process.nVisibilityWindowPos[0]= sb_window_posx->get_value_as_int();
	mConfig.process.nVisibilityWindowPos[1] = sb_window_posy->get_value_as_int();

	Draw_VisibilityWindow();
}

void DpcGUI::Draw_VisibilityWindow()
{
	if (mConfig.process.nVisibilityWindowPos[0]*mConfig.process.nVisibilityWindowPos[1]) {
		int w2=mConfig.process.nVisibilityWindow>>1;

		Gtk_addons::ImageViewerRectangle rect;
		int window=mConfig.process.nVisibilityWindow;

		rect.x      = mConfig.process.nVisibilityWindowPos[0]-w2;
		rect.y      = mConfig.process.nVisibilityWindowPos[1]-w2;
		rect.width  = mConfig.process.nVisibilityWindow;
		rect.height = mConfig.process.nVisibilityWindow;
		rect.color  = Gdk::Color("yellow");
		viewer_visibility.set_rectangle(rect,0);

		m_fVisibility=100.0f*m_pEngine->Visibility(window,mConfig.process.nVisibilityWindowPos);
		std::ostringstream msg;
		msg<<"Visibility of central "<<window<<"x"<<window<<" region: "<<setprecision(4)<<m_fVisibility<<"%";
		label_visibility->set_text(msg.str());
	}
}


// Modifiers
void DpcGUI::ToggleROIEntries()
{
	if(mConfig.projections.bUseROI) {
		sb_ROIx1->set_sensitive(true);
		sb_ROIy1->set_sensitive(true);
		sb_ROIx2->set_sensitive(true);
		sb_ROIy2->set_sensitive(true);

	} else {
		sb_ROIx1->set_sensitive(false);
		sb_ROIy1->set_sensitive(false);
		sb_ROIx2->set_sensitive(false);
		sb_ROIy2->set_sensitive(false);	
	}
}

void DpcGUI::ToggleNormROIEntries()
{
	if(mConfig.projections.bUseNorm) {
		sb_nROIx1->set_sensitive(true);
		sb_nROIy1->set_sensitive(true);
		sb_nROIx2->set_sensitive(true);
		sb_nROIy2->set_sensitive(true);

	} else {
		sb_nROIx1->set_sensitive(false);
		sb_nROIy1->set_sensitive(false);
		sb_nROIx2->set_sensitive(false);
		sb_nROIy2->set_sensitive(false);
	}
}

void DpcGUI::ToggleClampDFIEntries() 
{
	if(mConfig.process.bUseDFILimits) {
		sb_clamp_dfi_hi->set_sensitive(true);
		sb_clamp_dfi_lo->set_sensitive(true);
	} else {
		sb_clamp_dfi_hi->set_sensitive(false);
		sb_clamp_dfi_lo->set_sensitive(false);
	}
}

void DpcGUI::ToggleClampAmplitudeEntries() 
{
	if(mConfig.process.bUseAmplLimits) {
		sb_clamp_ampl_hi->set_sensitive(true);
		sb_clamp_ampl_lo->set_sensitive(true);
	} else {
		sb_clamp_ampl_hi->set_sensitive(false);
		sb_clamp_ampl_lo->set_sensitive(false);
	}
}
// Processors

void DpcGUI::SaveResult(kipl::base::TImage<float,2> & img, std::string mask, int index)
{
	std::string filename, ext;
	kipl::strings::filenames::MakeFileName(mask,mConfig.projections.nFirstIndex+index,filename, ext,'#','0');
	logger(kipl::logging::Logger::LogVerbose,filename);
	if (ext==".mat")
	  kipl::io::WriteMAT(img,(mConfig.projections.sDestPath+filename).c_str(),filename.c_str());
	else if (ext==".tif")
	  kipl::io::WriteTIFF32(img,(mConfig.projections.sDestPath+filename).c_str());
	else
	  throw nGIException("Unknown file type");
}

void DpcGUI::Preview()
{
	UpdateConfig();
	std::string tempdir = Glib::get_home_dir();

	kipl::strings::filenames::CheckPathSlashes(tempdir,true);
	tempdir=tempdir+"current_ngi.xml";
	std::ofstream conffile(tempdir.c_str());

	conffile<<mConfig.WriteXML(); conffile.close();

	nGIConfig cfg(mConfig);
	cfg.projections.nFilesCnt     = 1;
	cfg.process.bComputeAmplitude = true;
	cfg.process.bComputeDPC       = true;
	cfg.process.bComputeDFI       = true;
	cfg.process.bSerialize		  = false;
	cfg.projections.bUseROI       = cb_PreviewCropped->get_active();

	nGIFactory factory;
	if (m_pEngine!=NULL) {
		delete m_pEngine;
		m_pEngine=NULL;
	}

	Gtk::MessageDialog msgdlg("An error occurred",false,Gtk::MESSAGE_ERROR);
	try {
		m_pEngine=factory.BuildEngine(cfg,NULL);
		logger(kipl::logging::Logger::LogVerbose,"Building the Engine was successful.");
		m_pEngine->Run();
		logger(kipl::logging::Logger::LogVerbose,"The data was successfully processed.");
	}
	catch (kipl::base::KiplException & e) {
		logger(kipl::logging::Logger::LogError,e.what());
		msgdlg.run();
		msgdlg.hide();
		return;
	}
	catch (nGIException  &e) {
		logger(kipl::logging::Logger::LogError,e.what());
		msgdlg.run();
		msgdlg.hide();	
		return;
	}
	catch (ModuleException  &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
	catch (std::exception & e) {
		logger(kipl::logging::Logger::LogError,e.what());
		msgdlg.run();
		msgdlg.hide();
		return;
	}
	catch (...) {
		logger(kipl::logging::Logger::LogError,"Unknown error");
		msgdlg.run();
		msgdlg.hide();
		exit(-1);
	}

	ShowResults();
}

void DpcGUI::ShowResults()
{
	std::cout<<"show\n";
	Gtk::MessageDialog msgdlg("An error occurred",false,Gtk::MESSAGE_ERROR);
	kipl::base::TImage<float> trans;
	kipl::base::TImage<float> phase;
	kipl::base::TImage<float> dark;
	kipl::base::TImage<float> vis;

	try {
		m_pEngine->GetResults(trans,phase,dark,vis);
	}
	catch (nGIException  &e) {
		logger(kipl::logging::Logger::LogError,e.what());
		msgdlg.run();
		msgdlg.hide();
		return;
	}
	catch (ModuleException  &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
	catch (std::exception & e) {
		logger(kipl::logging::Logger::LogError,e.what());
		msgdlg.run();
		msgdlg.hide();
		return;
	}
	catch (...) {
		logger(kipl::logging::Logger::LogError,"Unknown error");
		msgdlg.run();
		msgdlg.hide();
		exit(-1);
	}

	viewer_DPC.set_image(phase.GetDataPtr(),phase.Dims());
	viewer_amplitude.set_image(trans.GetDataPtr(),trans.Dims());
	viewer_DFI.set_image(dark.GetDataPtr(),dark.Dims());

	const size_t nBins=2048;
	float axis[nBins];
	size_t hist[nBins];
	kipl::base::TImage<float,3> &proj=m_pEngine->GetProjections();
	size_t nLo=0,nHi=0;

	scale_projections->get_adjustment()->configure(0.0, 1.0, mConfig.projections.nPhaseSteps, 1.0, 1.0, 0.0);

	// Plot oscillations
	float proj_osc[2048];
	float ref_osc[2048];

	m_pEngine->OscillationPlot(axis,proj_osc,ref_osc);
	viewer_plot.plot(axis,proj_osc,mConfig.projections.nPhaseSteps, Gdk::Color("blue"),0,Gtk_addons::PlotGlyph_Square,"Sample");
	viewer_plot.plot(axis,ref_osc,mConfig.projections.nPhaseSteps, Gdk::Color("green"),1,Gtk_addons::PlotGlyph_Square,"Reference");

	Gtk_addons::ImageViewerRectangle rect;
	if (mConfig.projections.bUseROI==true) {
		if (!cb_PreviewCropped->get_active()) {
			rect.x      = mConfig.projections.nROI[0];
			rect.y      = mConfig.projections.nROI[1];
			rect.width  = mConfig.projections.nROI[2]-mConfig.projections.nROI[0];
			rect.height = mConfig.projections.nROI[3]-mConfig.projections.nROI[1];
			rect.color = Gdk::Color("yellow");
			viewer_amplitude.set_rectangle(rect,0);
			viewer_DFI.set_rectangle(rect,0);
			viewer_DPC.set_rectangle(rect,0);
		}
		else {
			viewer_amplitude.clear_rectangle(0);
			viewer_DFI.clear_rectangle(0);
			viewer_DPC.clear_rectangle(0);
		}
	}
	else {
		viewer_amplitude.clear_rectangle(0);
		viewer_DFI.clear_rectangle(0);
		viewer_DPC.clear_rectangle(0);
	}

	on_changed_normroi();
	viewer_visibility.set_image(vis.GetDataPtr(),vis.Dims());

	if (mConfig.process.nVisibilityWindowPos[0]*mConfig.process.nVisibilityWindowPos[1]==0) {
		mConfig.process.nVisibilityWindowPos[0]=vis.Size(0)/2;
		mConfig.process.nVisibilityWindowPos[1]=vis.Size(1)/2;
		sb_window_posx->set_value(mConfig.process.nVisibilityWindowPos[0]);
		sb_window_posy->set_value(mConfig.process.nVisibilityWindowPos[1]);
	}
	else {
		Draw_VisibilityWindow();
	}




}

void DpcGUI::ProcessAll()
{
	UpdateConfig();
	std::string tempdir = Glib::get_home_dir();

	kipl::strings::filenames::CheckPathSlashes(tempdir,true);
	tempdir=tempdir+"current_ngi.xml";
	std::ofstream conffile(tempdir.c_str());

	conffile<<mConfig.WriteXML(); conffile.close();

	kipl::base::TImage<float> img;

	if (mConfig.process.bComputeAmplitude || mConfig.process.bComputeDPC || mConfig.process.bComputeDFI) {
		Gtk::MessageDialog msgdlg("An error occurred",false,Gtk::MESSAGE_ERROR);
		double invcnt=1.0/static_cast<double>(mConfig.projections.nFilesCnt);

		nGIFactory factory;
		if (m_pEngine!=NULL) {
			delete m_pEngine;
			m_pEngine=NULL;
		}

		try {
			mConfig.process.bSerialize=true;
			m_pEngine=factory.BuildEngine(mConfig,NULL);
			logger(kipl::logging::Logger::LogVerbose,"Building the Engine was successful.");
			m_pEngine->Run();
			logger(kipl::logging::Logger::LogVerbose,"The data was successfully processed.");
		}
		catch (kipl::base::KiplException &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
		catch (nGIException &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
		catch (ModuleException  &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
		catch (std::exception &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
		catch (...) {
			logger(kipl::logging::Logger::LogError,"Unknown error");
			msgdlg.run();
			msgdlg.hide();
			exit(-1);
		}
//		progressbar->set_fraction(1.0);
	}
	else {
		Gtk::MessageDialog msgdlg("Please select the image types to compute",false,Gtk::MESSAGE_ERROR);

		msgdlg.run();
		msgdlg.hide();

	}

}

void DpcGUI::Process(nGIConfig &config)
{
	if (m_pEngine!=NULL) {
		delete m_pEngine;
		m_pEngine=NULL;
	}

	std::ostringstream msg;
	Gtk::MessageDialog factoryerrdlg("Failed to build processing engine.",Gtk::MESSAGE_ERROR);

	try {
		m_pEngine=m_Factory.BuildEngine(config,NULL);
	}
	catch (nGIException &e) {
		msg<<"The nGIFactory threw a nGIException:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		factoryerrdlg.run();
		factoryerrdlg.hide();
		return;
	}
	catch (ModuleException &e) {
		msg<<"The nGIFactory threw a ModuleException:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		factoryerrdlg.run();
		factoryerrdlg.hide();
		return;
	}
	catch (kipl::base::KiplException &e) {
		msg<<"The nGIFactory threw a KiplException:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		factoryerrdlg.run();
		factoryerrdlg.hide();
		return;
	}
	catch (std::exception &e) {
		msg<<"The nGIFactory threw an STL exception:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		factoryerrdlg.run();
		factoryerrdlg.hide();
		return;
	}
	catch (...) {
		msg<<"The nGIFactory threw an unknown exception:\n";
		logger(kipl::logging::Logger::LogError,msg.str());
		factoryerrdlg.run();
		factoryerrdlg.hide();
		return;
	}

	msg.str("");
	Gtk::MessageDialog engineerrdlg("The engine failed to process the data.",Gtk::MESSAGE_ERROR);

	try {
		m_pEngine->Run();
	}
	catch (nGIException &e) {
		msg<<"The nGIEngine threw a nGIException:\n"<<e.what();
	}
	catch (ModuleException &e) {
		msg<<"The nGIEngine threw a ModuleException:\n"<<e.what();
	}
	catch (kipl::base::KiplException &e) {
		msg<<"The nGIEngine threw a KiplException:\n"<<e.what();
	}
	catch (std::exception &e) {
		msg<<"The nGIEngine threw an STL exception:\n"<<e.what();
	}
	catch (...) {
		msg<<"The nGIEngine threw an unknown exception:\n";
	}

	if (!msg.str().empty()) {
		logger(kipl::logging::Logger::LogError,msg.str());
		engineerrdlg.run();
		engineerrdlg.hide();
		delete m_pEngine;
		m_pEngine=NULL;
		return;

	}
}

void DpcGUI::on_changed_roi()
{
	if (mConfig.projections.bUseROI==true) {
		if (!cb_PreviewCropped->get_active()) {
			Gtk_addons::ImageViewerRectangle rect;
			rect.x = sb_ROIx1->get_value_as_int();
			rect.y = sb_ROIy1->get_value_as_int();

			
			rect.width  = sb_ROIx2->get_value_as_int() - rect.x;
			rect.height = sb_ROIy2->get_value_as_int() - rect.y;
			rect.color  = Gdk::Color("yellow");
			viewer_amplitude.set_rectangle(rect,0);
			viewer_DFI.set_rectangle(rect,0);
			viewer_DPC.set_rectangle(rect,0);
		}
	}
}

void DpcGUI::on_changed_normroi()
{
	if (mConfig.projections.bUseNorm==true) {
		Gtk_addons::ImageViewerRectangle rect;
		rect.x = sb_nROIx1->get_value_as_int();
		rect.y = sb_nROIy1->get_value_as_int();

		rect.width  = sb_nROIx2->get_value_as_int() - rect.x;
		rect.height = sb_nROIy2->get_value_as_int() - rect.y;
		rect.color  = Gdk::Color("green");;


		if (cb_PreviewCropped->get_active()==true) {
			viewer_amplitude.clear_rectangle(1);
			viewer_DFI.clear_rectangle(1);
			viewer_DPC.clear_rectangle(1);

			rect.x-=sb_ROIx1->get_value_as_int();
			rect.y-=sb_ROIy1->get_value_as_int();
		}

		viewer_amplitude.set_rectangle(rect,1);
		viewer_DFI.set_rectangle(rect,1);
		viewer_DPC.set_rectangle(rect,1);

	}
	else {
	}
}

void DpcGUI::Menu_new ()
{
	sCurrentFileName.clear();
	sCurrentFileName="newscript.xml";

	nGIConfig cfg;
	mConfig=cfg;

	UpdateDialog();
	size_t dims[]={256,256};
	kipl::base::TImage<float,2> img(dims);
	viewer_DPC.set_image(img.GetDataPtr(),img.Dims());
}

void DpcGUI::Menu_load ()
{
	Gtk::FileChooserDialog dialog(*main_Window,"Open parameter file");
		
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

	ostringstream msg;
	Gtk::MessageDialog errdlg("Failed to open parameter file",Gtk::MESSAGE_ERROR);
	if ( dialog.run()== Gtk::RESPONSE_OK)
	  {
		  std::string fname=dialog.get_filename();
		  try {
		  mConfig.LoadConfigFile(fname,"ngi");
		  }
		  catch (nGIException &e) {
			  msg<<"Failed to open "<< fname<<"\n"<<e.what();
			  logger(kipl::logging::Logger::LogError,msg.str());
			  errdlg.run();
			  errdlg.hide();
			  return;
		  }
		  catch (kipl::base::KiplException &e) {
			  msg<<"Failed to open "<< fname<<"\n"<<e.what();
			  logger(kipl::logging::Logger::LogError,msg.str());
			  errdlg.run();
			  errdlg.hide();
			  return;
		  }

		  catch (std::exception &e) {
			  msg<<"Failed to open "<< fname<<"\n"<<e.what();
			  logger(kipl::logging::Logger::LogError,msg.str());
			  errdlg.run();
			  errdlg.hide();
			  return;
		  }

		  catch (ModuleException &e) {
			  msg<<"Failed to open "<< fname<<"\n"<<e.what();
			  logger(kipl::logging::Logger::LogError,msg.str());
			  errdlg.run();
			  errdlg.hide();
			  return;
		  }
		  catch (...) {
			  msg<<"Failed to open "<< fname<<", unknown exception";
			  logger(kipl::logging::Logger::LogError,msg.str());
			  errdlg.run();
			  errdlg.hide();
			  return;
		  }

		  sCurrentFileName=fname;
		  UpdateDialog();	
		  ToggleROIEntries();
		  ToggleNormROIEntries();
		  ToggleClampDFIEntries();
		  ToggleClampAmplitudeEntries();
	  }		
}

void DpcGUI::Menu_save ()
{
	if (sCurrentFileName=="newscript.xml" || (sCurrentFileName.empty()==true)) {
		Menu_save_as();
	}
	else {
		std::ofstream parfile(sCurrentFileName.c_str());

		UpdateConfig();
		parfile<<mConfig.WriteXML()<<std::endl;
	}
}

void DpcGUI::Menu_save_as()
{
	Gtk::FileChooserDialog dialog(*main_Window,"Save parameter file as",Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_filename(sCurrentFileName);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

	if ( dialog.run()== Gtk::RESPONSE_OK)
	  {
		  UpdateConfig();
		  sCurrentFileName=dialog.get_filename();;
		  
		  ofstream of(sCurrentFileName.c_str());
		  of<<mConfig.WriteXML();  	
	  }		
}

void DpcGUI::CB_menu_quit ()
{
	main_Window->hide();
}

void DpcGUI::CB_menu_createreport()
{
	Gtk::FileChooserDialog dialog(*main_Window,"Save report as",Gtk::FILE_CHOOSER_ACTION_SAVE);
	
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

	if ( dialog.run()== Gtk::RESPONSE_OK)
	  {
		Gtk::MessageDialog msgdlg("An error occurred",false,Gtk::MESSAGE_ERROR);
		kipl::base::TImage<float> trans;
		kipl::base::TImage<float> phase;
		kipl::base::TImage<float> dark;
		kipl::base::TImage<float> vis;
		float axis[2048];
		float proj_osc[2048];
		float ref_osc[2048];

		try {
			m_pEngine->GetResults(trans,phase,dark,vis);
			m_pEngine->OscillationPlot(axis,proj_osc,ref_osc);
		}
		catch (nGIException  &e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
		catch (ModuleException  &e) {
				logger(kipl::logging::Logger::LogError,e.what());
				msgdlg.run();
				msgdlg.hide();
				return;
			}
		catch (std::exception & e) {
			logger(kipl::logging::Logger::LogError,e.what());
			msgdlg.run();
			msgdlg.hide();
			return;
		}
		catch (...) {
			logger(kipl::logging::Logger::LogError,"Unknown error");
			msgdlg.run();
			msgdlg.hide();
			exit(-1);
		}

		  std::string sReportFileName=dialog.get_filename();;
		  int dims[]={595,842};
		  nGIReportGenerator report(dims);
		  if (sCurrentFileName.empty()) {
			  report.CreateReport(sReportFileName, "NoName",&mConfig,trans,phase,dark,m_fVisibility,axis,proj_osc,ref_osc);
		  }
		  else {
			report.CreateReport(sReportFileName, sCurrentFileName,&mConfig,trans,phase,dark,m_fVisibility,axis,proj_osc,ref_osc);
		  }
	  }			
}

void DpcGUI::DisplayNewProjection()
{
	if (m_pEngine!=NULL) {
		int slice_number=static_cast<int>(scale_projections->get_value())-1;

		kipl::base::TImage<float,3> &projections=m_pEngine->GetProjections();

		float lo, hi;

		viewer_projections.get_levels(&lo,&hi);
	//	size_t N=projections.Size(0)*projections.Size(1);
		float *pSlice=projections.GetLinePtr(0,slice_number);
		//hi=*max(pSlice,pSlice+N);
		//lo=*min(pSlice,pSlice+N);

		viewer_projections.set_image(pSlice, projections.Dims(), lo, hi);
	}
}


