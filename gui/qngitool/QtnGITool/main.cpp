#include <QtWidgets/QApplication>
#include <QString>
#include <QVector>

#include <sstream>

#include "ngimainwindow.h"
#include "ngireport.h"

#include <ModuleException.h>

#include <base/KiplException.h>
#include <strings/filenames.h>
#include <utilities/nodelocker.h>
#include <logging/logger.h>

#include <nGIConfig.h>
#include <nGIFactory.h>
#include <nGIEngine.h>
#include <nGIException.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int RunGUI(QApplication *app);
int RunOffline(QApplication *app);

int main(int argc, char *argv[])
{
    kipl::logging::Logger logger("nGITool:main");
    std::ostringstream msg;
    QApplication app(argc, argv);

    std::string homedir = QDir::homePath().toStdString();

    kipl::strings::filenames::CheckPathSlashes(homedir,true);

    std::string application_path=app.applicationDirPath().toStdString();

    kipl::strings::filenames::CheckPathSlashes(application_path,true);
//#ifndef __APPLE__
#ifndef NEVER
    kipl::utilities::NodeLocker license(homedir);
    bool licensefail=false;
    std::string errormsg;
    try {
        std::list<std::string> liclist;
        liclist.push_back(homedir+".imagingtools/license_ngitool.dat");
        liclist.push_back(application_path+"license_ngitool.dat");
        liclist.push_back(application_path+"license.dat");
        liclist.push_back(homedir+"license_ngitool.dat");

        license.Initialize(liclist,"ngitool");
    }
    catch (kipl::base::KiplException &e) {
        errormsg=e.what();
        licensefail=true;
    }

    if (licensefail || !license.AccessGranted()) {
        msg.str("");
        if (licensefail)
            msg<<"Could not locate the license file\n"<<errormsg<<"\n";
        else
            msg<<"nGITool is not registered on this computer\n";

        msg<<"\nPlease activate nGITool on http://www.imagingscience.ch/usermanager.\n";
        msg<<"\nActivation code: "<<*license.GetNodeString().begin();
        logger(kipl::logging::Logger::LogError,msg.str());
        QMessageBox mbox;

        mbox.addButton("Register",QMessageBox::AcceptRole);
        mbox.addButton(QMessageBox::Save);
        mbox.addButton(QMessageBox::Abort);
        mbox.setText(QString::fromStdString(msg.str()));
        mbox.setWindowTitle("License error");
        mbox.setDetailedText(QString::fromStdString(license.GetMessage()));
        int res=mbox.exec();
        std::cout<<"Res ="<<res<<std::endl;
        if (res==QMessageBox::Save) {
            QDir dir;
            QString fname=QFileDialog::getOpenFileName(&mbox,"Select the license file",dir.homePath(),"*.dat");

            if (!fname.isEmpty()) {
                if (!dir.exists(dir.homePath()+"/.imagingtools")) {
                    dir.mkdir(QDir::homePath()+"/.imagingtools");
                }
                std::cout<<(dir.homePath()+"/.imagingtools/license_muhrec.dat").toStdString()<<std::endl;
                QFile::copy(fname,dir.homePath()+"/.imagingtools/license_muhrec.dat");
            }
        }
        if (res==QMessageBox::Accept) {
            logger(kipl::logging::Logger::LogMessage,"Opening default web browser.");
        }

        return -1;
    }
#endif


    if (app.arguments().size()==1)
        return RunGUI(&app);
    else
        return RunOffline(&app);

    return 0;
}

int RunGUI(QApplication *app)
{
  std::ostringstream msg;
  kipl::logging::Logger logger("QnGITool::RunGUI");

  try {
      nGIMainWindow w(app);
      w.show();

      return app->exec();
  }
  catch (nGIException &e) {
      msg<<"An nGI exception was thrown during the main window initialization\n"<<e.what();
      logger(kipl::logging::Logger::LogError,msg.str());

      return -1;
  }
  catch (ModuleException &e) {
          msg<<"A module exception was thrown during the main window initialization\n"<<e.what();
          logger(kipl::logging::Logger::LogError,msg.str());

          return -6;
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
  catch (...) {
      msg<<"An unknown exception was thrown\n";
      logger(kipl::logging::Logger::LogError,msg.str());
      return -7;
  }
  return -100;
}

int RunOffline(QApplication *app)
{
    std::ostringstream msg;
    kipl::logging::Logger logger("QnGITool::RunOffline");
#ifdef _OPENMP
    omp_set_nested(1);
#endif
    QVector<QString> args=app->arguments().toVector();
    if (2<args.size()) {
        if (args[1]!="-f") {
          logger(kipl::logging::Logger::LogMessage,"QnGITool is running in offline mode");
          try {
                  nGIFactory factory;
                  logger(kipl::logging::Logger::LogMessage, "Building the processing engine");
                  nGIConfig config;
                  config.LoadConfigFile(args[2].toStdString(),"ngi");
                  config.process.bSerialize=true;
                  nGIEngine *pEngine=factory.BuildEngine(config,NULL);
                  if (pEngine!=NULL) {
                          logger(kipl::logging::Logger::LogMessage, "Starting processing");
                          pEngine->Run();
                          logger(kipl::logging::Logger::LogMessage, "Processing done");
                  }
                  else {
                          logger(kipl::logging::Logger::LogMessage, "There is no processing engine, skipping processing");
                  }
              }  // Exception handling as last resort to report unhandled faults
              catch (nGIException &re) {
                  cerr<<"An unhandled ngi exception occurred"<<endl;
                  cerr<<"Trace :"<<endl<<re.what()<<endl;
                  return -1;
              }
              catch (ModuleException &e) {
                    msg<<"A module exception was thrown during engine initialization\n"<<e.what();
                    logger(kipl::logging::Logger::LogError,msg.str());

                    return -3;
                }
              catch (kipl::base::KiplException &ke) {
                  cerr<<"An unhandled kipl exception occurred"<<endl;
                  cerr<<"Trace :"<<endl<<ke.what()<<endl;
                  return -2;
              }

              catch (std::exception &e) {
                  cerr<<"An unhandled STL exception occurred"<<endl;
                  cerr<<"Trace :"<<endl<<e.what()<<endl;
                  return -4;
              }

              catch (...) {
                  cerr<<"An unknown exception occurred"<<endl;
                  return -5;
              }
          }
      }

    return 0;
}
