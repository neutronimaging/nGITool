//============================================================================
// Name        : dpc.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Main program for the DPC processing application
//============================================================================

//#include <gtkmm/main.h>

#include "stdafx.h"
#include <gtkmm.h>
#include <iostream>
#include "../include/DpcGUI.h"
#include "../include/nGIReportGenerator.h"
#include <nGIConfig.h>
#include <nGIProcessor.h>
#include <nGIException.h>
#include <ModuleException.h>
#include <base/KiplException.h>
#include <strings/filenames.h>
#include <utilities/nodelocker.h>
#include <logging/logger.h>
#include <sstream>
#include <string>

using namespace std;

void testconfig();
int RunCommandLine(int argc, char *argv[]);
int RunGUI(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	kipl::logging::Logger logger("nGI tool");
	kipl::logging::Logger::SetLogLevel(kipl::logging::Logger::LogVerbose);

	std::stringstream msg;

	logger(kipl::logging::Logger::LogMessage,"Checking license...");
	std::string tempdir = Glib::get_home_dir();
	kipl::strings::filenames::CheckPathSlashes(tempdir,true);
	kipl::utilities::NodeLocker license(tempdir);
	bool licensefail=false;
	std::string errormsg;

	try {
		std::list<std::string> licfiles;
		licfiles.push_back(".\\license_ngitool.dat");
		licfiles.push_back(".\\license.dat");
		licfiles.push_back(tempdir+"license_ngitool.dat");
		licfiles.push_back(tempdir+"license.dat");
		licfiles.push_back(tempdir+"licenses\\license_ngitool.dat");
		licfiles.push_back(tempdir+"licenses\\license.dat");

		license.Initialize(licfiles,"ngitool");
	}
	catch (kipl::base::KiplException & e) {
		errormsg=e.what();
		licensefail=true;
	}

	if (licensefail || !license.AccessGranted()) {
	 Gtk::Main kit(argc, argv);

	 msg.str("");
	 if (licensefail)
		msg<<"Could not locate the license file\n"<<errormsg<<"\n";
	 else
		msg<<"nGI tool is not registered on this computer\n";

	 msg<<"\nPlease contact Anders Kaestner (anders.kaestner@psi.ch) to activate the nGI tool.\n";
	 msg<<"\nActivation code: "<<license.GetNodeString().front();
	 logger(kipl::logging::Logger::LogError,msg.str());
	 Gtk::MessageDialog msgdlg(msg.str(),false,Gtk::MESSAGE_ERROR);
	 msgdlg.set_title("License error");
	 msgdlg.run();
	 return -1;
	}


	if (1<argc) {
		if (!strcmp(argv[1],"-t"))
			test_nGIReport();

		if (!strcmp(argv[1],"-f"))
			RunCommandLine(argc,argv);

		return 0;
	}
	else
		RunGUI(argc,argv);


  return 0;
}

int RunGUI(int argc, char * argv[])
{
	ostringstream msg;
	kipl::logging::Logger logger("nGI Tool cmd");

	logger(kipl::logging::Logger::LogMessage,"Starting GUI");
	Gtk::Main kit(argc, argv);

	std::string application_path;
	char *tmpstr = getenv("NGI_BASE_PATH");
	if (tmpstr!=NULL) {
		application_path=tmpstr;
		kipl::strings::filenames::CheckPathSlashes(application_path,true);
	}

	Glib::RefPtr<Gtk::Builder> refBuilder =Gtk::Builder::create();
	try {
	#ifdef _MSC_VER
		std::string gladefile=application_path+"resources/dpc_gtk24.glade";
	#else
		std::string gladefile=application_path+"resources/dpc.glade";
	#endif

		kipl::strings::filenames::CheckPathSlashes(gladefile,false);

		refBuilder->add_from_file(gladefile);
	}
	catch (const Glib::FileError &ex){
		std::cerr<<"File error: "<<ex.what()<<std::endl;
		return 1;
	}
	catch (const Gtk::BuilderError &ex) {
		std::cerr<<"BuilderError: "<<ex.what()<<std::endl;
		return 1;
	}
	catch (kipl::base::KiplException &e) {
		  msg<<"A kipl exception was thrown during the main window initialization\n"<<e.what();
		  logger(kipl::logging::Logger::LogError,msg.str());

		  return -2;
	}
	catch (std::exception &e) {
		msg<<"An STL exception was thrown during the main window initialization\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());

		return -3;
	}
	catch (Glib::Exception & e) {
		msg<<"An unhandled Glib exception occurred\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		return -4;
	}
	catch (...) {
		  msg<<"An unknown exception was thrown\n";
		  logger(kipl::logging::Logger::LogError,msg.str());
		  return -5;
	}

	try {
		DpcGUI mainwindow(refBuilder, application_path);

		kit.run(mainwindow.get_window());
	}
	catch (kipl::base::KiplException &e) {
		msg<<"A kipl exception was thrown during the main window initialization\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());

		return -2;
	}
	catch (std::exception &e) {
		msg<<"An STL exception was thrown during the main window initialization\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());

		return -2;
	}

	return 0;
}

int RunCommandLine(int argc, char *argv[])
{
	ostringstream msg;
	kipl::logging::Logger logger("nGI Tool cmd");

	if (argc<3) {
		logger(kipl::logging::Logger::LogError,"Config file missing");
		return -1;
	}

	std::string fname=argv[2];

	nGIConfig config;

	msg.str("");
	try {
		config.LoadConfigFile(fname,"ngi");
	}
	catch (nGIException &e) {
		msg<<"Failed to load the config file: \n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		return -2;
	}
	catch (ModuleException &e) {
		msg<<"Failed to load the config file: \n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		return -2;
	}
	catch (kipl::base::KiplException &e) {
		msg<<"Failed to load the config file: \n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		return -2;
	}
	catch (std::exception &e) {
		msg<<"Failed to load the config file: \n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
		return -2;
	}
	catch (...) {
		msg<<"Failed to load the config file: \n";
		logger(kipl::logging::Logger::LogError,msg.str());
		return -2;
	}

	nGIFactory factory;
	nGIEngine *engine=NULL;

	msg.str("");
	try {
		config.process.bSerialize=true;
		engine=factory.BuildEngine(config,NULL);
		logger(kipl::logging::Logger::LogVerbose,"Building the Engine was successful.");

	}
	catch (nGIException &e) {
		msg<<"The nGIFactory threw a nGIException:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
	}
	catch (ModuleException &e) {
		msg<<"The nGIFactory threw a ModuleException:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
	}
	catch (kipl::base::KiplException &e) {
		msg<<"The nGIFactory threw a KiplException:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
	}
	catch (std::exception &e) {
		msg<<"The nGIFactory threw an STL exception:\n"<<e.what();
		logger(kipl::logging::Logger::LogError,msg.str());
	}
	catch (...) {
		msg<<"The nGIFactory threw an unknown exception:\n";
		logger(kipl::logging::Logger::LogError,msg.str());
	}
	if (!msg.str().empty()) {
		logger(kipl::logging::Logger::LogError,msg.str());
		delete engine;
		return -3;

	}

	msg.str("");
	try {
		engine->Run();
		logger(kipl::logging::Logger::LogVerbose,"Processing finished successfully");
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
		if (engine!=NULL)
			delete engine;
		return -4;
	}


	return 0;
}

void testconfig()
{
	nGIConfig config;

//	config.bUseNorm=true;
//	config.bCompletePeriod=true;
//	config.bUseROI=true;
//	config.nFilesCnt=10;
//	config.nFirstIndex=12;
//	config.nNormROI[0]=1; config.nNormROI[1]=2; config.nNormROI[2]=3; config.nNormROI[3]=4;
//	config.nROI[0]=5; config.nROI[1]=6; config.nROI[2]=7; config.nROI[3]=8;
//	config.nPhaseFiles=11;
//	config.sDarkMask="hepp";
//	config.sDestMask="hopp";
//	config.sDestPath="happ";
//	config.sFileMask="hipp";
//	config.sObMask="tjipp";
//	config.sPath="bopp";
//	config.sReferencePath="blopp";
//
//	cout<<config.WriteXML()<<endl;
//
//	config.ParseXML("config.xml");
//	cout<<endl<<config.WriteXML()<<endl;

}

