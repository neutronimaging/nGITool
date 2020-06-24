#include <QtWidgets/QApplication>
#include <QDesktopServices>
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

  try
  {
      nGIMainWindow w(app);
      w.show();

      return app->exec();
  }
  catch (nGIException &e)
  {
      msg<<"An nGI exception was thrown during the main window initialization\n"<<e.what();
      logger(kipl::logging::Logger::LogError,msg.str());

      return -1;
  }
  catch (ModuleException &e)
  {
          msg<<"A module exception was thrown during the main window initialization\n"<<e.what();
          logger(kipl::logging::Logger::LogError,msg.str());

          return -6;
  }
  catch (kipl::base::KiplException &e)
  {
      msg<<"A kipl exception was thrown during the main window initialization\n"<<e.what();
      logger(kipl::logging::Logger::LogError,msg.str());

      return -2;
  }
  catch (std::exception &e)
  {
    msg<<"An STL exception was thrown during the main window initialization\n"<<e.what();
    logger(kipl::logging::Logger::LogError,msg.str());

    return -3;
  }
  catch (...)
  {
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

    if (2<args.size())
    {
        if (args[1]!="-f")
        {
            logger(kipl::logging::Logger::LogMessage,"QnGITool is running in offline mode");
            try
            {
                nGIFactory factory;
                logger(kipl::logging::Logger::LogMessage, "Building the processing engine");
                nGIConfig config(app->applicationDirPath().toStdString());
                config.LoadConfigFile(args[2].toStdString(),"ngi");
                config.process.bSerialize=true;
                nGIEngine *pEngine=factory.BuildEngine(config,nullptr);
                if (pEngine!=nullptr)
                {
                      logger(kipl::logging::Logger::LogMessage, "Starting processing");
                      pEngine->Run();
                      logger(kipl::logging::Logger::LogMessage, "Processing done");
                }
                else
                {
                      logger(kipl::logging::Logger::LogMessage, "There is no processing engine, skipping processing");
                }
            }  // Exception handling as last resort to report unhandled faults
            catch (nGIException &re)
            {
                cerr<<"An unhandled ngi exception occurred"<<endl;
                cerr<<"Trace :"<<endl<<re.what()<<endl;
                return -1;
            }
            catch (ModuleException &e)
            {
                msg<<"A module exception was thrown during engine initialization\n"<<e.what();
                logger(kipl::logging::Logger::LogError,msg.str());

                return -3;
            }
            catch (kipl::base::KiplException &ke)
            {
                cerr<<"An unhandled kipl exception occurred"<<endl;
                cerr<<"Trace :"<<endl<<ke.what()<<endl;
                return -2;
            }
            catch (std::exception &e)
            {
                cerr<<"An unhandled STL exception occurred"<<endl;
                cerr<<"Trace :"<<endl<<e.what()<<endl;
                return -4;
            }
            catch (...)
            {
                cerr<<"An unknown exception occurred"<<endl;
                return -5;
            }
        }
    }

    return 0;
}
