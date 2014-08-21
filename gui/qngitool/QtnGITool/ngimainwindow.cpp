#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QString>
#include <QMessageBox>
#include <QRect>

#include "ngimainwindow.h"
#include "ui_ngimainwindow.h"
#include "ngireport.h"
#include <nGIException.h>
#include <nGIFactory.h>
#include <base/KiplException.h>
#include <ModuleException.h>

#include <io/DirAnalyzer.h>
#include <strings/filenames.h>

#include <fstream>
#include <sstream>
#include <string>



nGIMainWindow::nGIMainWindow(QApplication *app, QWidget *parent) :
    QMainWindow(parent),
    logger("nGIMainWindow"),
    m_QtApp(app),
    ui(new Ui::nGIMainWindow),
    m_pEngine(NULL)
{
    ui->setupUi(this);
    logger.AddLogTarget(*(ui->logger));
    LoadDefaults();
    on_checkCropImages_toggled(ui->checkCropImages->checkState());
    on_checkDoseRegion_toggled(ui->checkDoseRegion->checkState());
    on_checkClampTransmission_toggled(ui->checkClampTransmission->checkState());
    on_checkClampDFI_toggled(ui->checkClampDFI->checkState());
}

nGIMainWindow::~nGIMainWindow()
{
    delete ui;
}

void nGIMainWindow::LoadDefaults()
{
    std::string defaultsname;
    QDir dir;
    QString currentname=dir.homePath()+"/.imagingtools/CurrentnGI.xml";

    bool bUseDefaults=true;
    if (dir.exists(currentname)) {
        defaultsname=currentname.toStdString();
        bUseDefaults=false;
    }
    else {
    #ifdef Q_OS_WIN32
         defaultsname="resources/defaults_windows.xml";
    #elseif Q_OS_LINUX
        defaultsname=m_sApplicationPath+"resources/defaults_linux.xml";
    #elseif Q_OS_DARWIN
        defaultsname=m_sApplicationPath+"../Resources/defaults_mac.xml";
    #endif
        bUseDefaults=true;
    }
    std::ostringstream msg;

    kipl::strings::filenames::CheckPathSlashes(defaultsname,false);
    try {
        m_Config.LoadConfigFile(defaultsname.c_str(),"ngitool");
        msg.str(""); msg<<"Loaded config file "<<defaultsname;
        logger(kipl::logging::Logger::LogMessage, msg.str());
    }
    catch (nGIException &e) {
        msg<<"Loading defaults failed :\n"<<e.what();
        logger(kipl::logging::Logger::LogError,msg.str());
    }
    catch (ModuleException &e) {
        msg<<"Loading defaults failed :\n"<<e.what();
        logger(kipl::logging::Logger::LogError,msg.str());
    }
    catch (kipl::base::KiplException &e) {
        msg<<"Loading defaults failed :\n"<<e.what();
        logger(kipl::logging::Logger::LogError,msg.str());
    }

    if (bUseDefaults) {
        m_Config.projections.sProjectionMask       = QDir::homePath().toStdString();
        m_Config.projections.sReferenceMask        = QDir::homePath().toStdString();
        m_Config.projections.sDestPath             = QDir::homePath().toStdString();

        std::list<ModuleConfig>::iterator it;

        std::string sSearchStr = "@executable_path";
        std::string sModulePath=m_QtApp->applicationDirPath().toStdString();
        size_t pos=0;
        for (it=m_Config.modules.begin(); it!=m_Config.modules.end(); it++) {
            pos=it->m_sSharedObject.find(sSearchStr);
            logger(kipl::logging::Logger::LogMessage,it->m_sSharedObject);
            if (pos!=std::string::npos)
                it->m_sSharedObject.replace(pos,sSearchStr.size(),sModulePath);
            logger(kipl::logging::Logger::LogMessage,it->m_sSharedObject);
        }
    }

    UpdateDialog();
}

void nGIMainWindow::on_actionNew_triggered() {
    logger(kipl::logging::Logger::LogMessage,"Requested new config");
    nGIConfig conf;
    m_Config=conf;
    UpdateDialog();
}

void nGIMainWindow::on_actionOpen_triggered()
{
    logger(kipl::logging::Logger::LogMessage,"Load saved configuration");
    std::ostringstream msg;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open nGI processing configuration"),tr("configurations (*.xml)"));

    QMessageBox msgbox;

    msgbox.setWindowTitle(tr("Config file error"));
    msgbox.setText(tr("Failed to load the configuration file"));

    try {
        m_Config.LoadConfigFile(fileName.toStdString(),"reconstructor");
    }
    catch (nGIException & e) {
        msg<<"Failed to load the configuration file :\n"<<
             e.what();
        msgbox.setDetailedText(QString::fromStdString(msg.str()));
        msgbox.exec();
    }
    catch (ModuleException & e) {
        msg<<"Failed to load the configuration file :\n"<<
             e.what();
        msgbox.setDetailedText(QString::fromStdString(msg.str()));
        msgbox.exec();
    }
    catch (kipl::base::KiplException & e) {
        msg<<"Failed to load the configuration file :\n"<<
             e.what();
        msgbox.setDetailedText(QString::fromStdString(msg.str()));
        msgbox.exec();
    }
    catch (std::exception & e) {
        msg<<"Failed to load the configuration file :\n"<<
             e.what();
        msgbox.setDetailedText(QString::fromStdString(msg.str()));
        msgbox.exec();
    }

    UpdateDialog();
}
void nGIMainWindow::on_actionSave_triggered()
{
    logger(kipl::logging::Logger::LogMessage,"Save configuration");
    if (m_sConfigFilename=="noname.xml")
        on_actionSave_as_triggered();
    else {
        std::ofstream conffile(m_sConfigFilename.c_str());

        conffile<<m_Config.WriteXML();
    }
}

void nGIMainWindow::on_actionSave_as_triggered()
{
    logger(kipl::logging::Logger::LogMessage,"Saveing configuration as");
    QString fname=QFileDialog::getSaveFileName(this,"Save configuration file",QDir::homePath());

    m_sConfigFilename=fname.toStdString();
    std::ofstream conffile(m_sConfigFilename.c_str());

    conffile<<m_Config.WriteXML();
}

void nGIMainWindow::on_actionQuit_triggered()
{
    SaveCurrentSetup();

    logger(kipl::logging::Logger::LogMessage,"Quit triggered");

    m_QtApp->quit();
}

void nGIMainWindow::on_actionPrint_triggered()
{
    logger(kipl::logging::Logger::LogMessage,"Printing report");
    QString fname=QFileDialog::getSaveFileName(this,"Save configuration file",QDir::homePath());

    nGIReport report;

    //report.CreateReport();
}


void nGIMainWindow::on_checkCropImages_toggled(bool checked)
{
    ui->spinCrop0->setEnabled(checked);
    ui->spinCrop1->setEnabled(checked);
    ui->spinCrop2->setEnabled(checked);
    ui->spinCrop3->setEnabled(checked);
    ui->labelCrop0->setEnabled(checked);
    ui->labelCrop1->setEnabled(checked);
    ui->labelCrop2->setEnabled(checked);
    ui->labelCrop3->setEnabled(checked);
    ui->buttonGetCropROI->setEnabled(checked);
    if (checked) {
        ui->imageProjections->set_rectangle(m_CurrentCropROI,Qt::green,0);
    }
    else {
        ui->imageProjections->clear_rectangle(0);
    }
}

void nGIMainWindow::on_buttonGetCropROI_clicked()
{
    m_CurrentCropROI=ui->imageProjections->get_marked_roi();

    ui->spinCrop0->setValue(m_CurrentCropROI.x());
    ui->spinCrop1->setValue(m_CurrentCropROI.y());
    ui->spinCrop2->setValue(m_CurrentCropROI.y()+m_CurrentCropROI.width());
    ui->spinCrop3->setValue(m_CurrentCropROI.y()+m_CurrentCropROI.height());

    ui->imageProjections->set_rectangle(m_CurrentCropROI,Qt::green,0);
}

void nGIMainWindow::on_buttonGetDoseROI_clicked()
{
    m_CurrentDoseROI=ui->imageProjections->get_marked_roi();

    ui->spinDose0->setValue(m_CurrentDoseROI.x());
    ui->spinDose1->setValue(m_CurrentDoseROI.y());
    ui->spinDose2->setValue(m_CurrentDoseROI.y()+m_CurrentDoseROI.width());
    ui->spinDose3->setValue(m_CurrentDoseROI.y()+m_CurrentDoseROI.height());

    ui->imageProjections->set_rectangle(m_CurrentDoseROI,Qt::yellow,1);
}

void nGIMainWindow::on_checkDoseRegion_toggled(bool checked)
{
    ui->spinDose0->setEnabled(checked);
    ui->spinDose1->setEnabled(checked);
    ui->spinDose2->setEnabled(checked);
    ui->spinDose3->setEnabled(checked);
    ui->labelDose0->setEnabled(checked);
    ui->labelDose1->setEnabled(checked);
    ui->labelDose2->setEnabled(checked);
    ui->labelDose3->setEnabled(checked);
    ui->buttonGetDoseROI->setEnabled(checked);
    if (checked) {
        ui->imageProjections->set_rectangle(m_CurrentDoseROI,Qt::yellow,1);
    }
    else {
        ui->imageProjections->clear_rectangle(1);
    }
}

void nGIMainWindow::on_buttonBrowseProjection_clicked()
{
    std::string pdir = ui->editProjectionMask->text().toStdString();

    #ifdef _MSC_VER
        const char slash='\\';
    #else
        const char slash='/';
    #endif
    ptrdiff_t pos=pdir.find_last_of(slash);

    QString path(QString::fromStdString(pdir.substr(0,pos+1)));


    QString projdir=QFileDialog::getOpenFileName(this,
                                      "Select location of the projections",
                                      path);
    if (!projdir.isEmpty()) {
        std::string fname=projdir.toStdString();

        kipl::io::DirAnalyzer da;
        kipl::io::FileItem fi=da.GetFileMask(fname);

        ui->editProjectionMask->setText(QString::fromStdString(fi.m_sMask));
    }
}

void nGIMainWindow::on_buttonReferenceBrowse_clicked()
{
    std::string pdir = ui->editReferenceMask->text().toStdString();

    #ifdef _MSC_VER
        const char slash='\\';
    #else
        const char slash='/';
    #endif
    ptrdiff_t pos=pdir.find_last_of(slash);

    QString path(QString::fromStdString(pdir.substr(0,pos+1)));


    QString projdir=QFileDialog::getOpenFileName(this,
                                      "Select location of the reference scan",
                                      path);
    if (!projdir.isEmpty()) {
        std::string fname=projdir.toStdString();

        kipl::io::DirAnalyzer da;
        kipl::io::FileItem fi=da.GetFileMask(fname);

        ui->editReferenceMask->setText(QString::fromStdString(fi.m_sMask));
    }
}

void nGIMainWindow::on_buttonDarkBrowse_clicked()
{
    std::string pdir = ui->editDarkMask->text().toStdString();

    #ifdef _MSC_VER
        const char slash='\\';
    #else
        const char slash='/';
    #endif
    ptrdiff_t pos=pdir.find_last_of(slash);

    QString path(QString::fromStdString(pdir.substr(0,pos+1)));


    QString projdir=QFileDialog::getOpenFileName(this,
                                      "Select location of the dark current images",
                                      path);
    if (!projdir.isEmpty()) {
        std::string fname=projdir.toStdString();

        kipl::io::DirAnalyzer da;
        kipl::io::FileItem fi=da.GetFileMask(fname);

        ui->editDarkMask->setText(QString::fromStdString(fi.m_sMask));
    }
}

void nGIMainWindow::on_buttonDestinationBrowse_clicked()
{
    QString path=ui->editDestinationPath->text();
    QString projdir=QFileDialog::getExistingDirectory(this,
                                      "Select the destination of the processed data",
                                      path);
    if (!projdir.isEmpty()) {
        ui->editDestinationPath->setText(projdir);
    }
}

void nGIMainWindow::on_buttonCreateReport_clicked()
{
    on_actionPrint_triggered();
}

void nGIMainWindow::on_buttonProcessAll_clicked()
{

}

void nGIMainWindow::on_buttonPreview_clicked()
{
    UpdateConfig();

    SaveCurrentSetup();

    nGIConfig cfg(m_Config);
    cfg.projections.nFilesCnt     = 1;
    cfg.process.bComputeAmplitude = true;
    cfg.process.bComputeDPC       = true;
    cfg.process.bComputeDFI       = true;
    cfg.process.bSerialize		  = false;
    cfg.projections.bUseROI       = ui->checkPreviewCropped->checkState();

    nGIFactory factory;
    if (m_pEngine!=NULL) {
        delete m_pEngine;
        m_pEngine=NULL;
    }

    QMessageBox msgdlg;
    msgdlg.setText("There was an error during processing your data");
    try {
        m_pEngine=factory.BuildEngine(cfg,NULL);
        logger(kipl::logging::Logger::LogVerbose,"Building the Engine was successful.");
        m_pEngine->Run();
        logger(kipl::logging::Logger::LogVerbose,"The data was successfully processed.");
    }
    catch (kipl::base::KiplException & e) {
        logger(kipl::logging::Logger::LogError,e.what());
        msgdlg.setDetailedText(QString::fromStdString(e.what()));
        msgdlg.exec();
        return;
    }
    catch (nGIException  &e) {
        logger(kipl::logging::Logger::LogError,e.what());
        msgdlg.setDetailedText(QString::fromStdString(e.what()));
        msgdlg.exec();
        return;
    }
    catch (ModuleException  &e) {
            logger(kipl::logging::Logger::LogError,e.what());
            msgdlg.setDetailedText(QString::fromStdString(e.what()));
            msgdlg.exec();
            return;
        }
    catch (std::exception & e) {
        logger(kipl::logging::Logger::LogError,e.what());
        msgdlg.setDetailedText(QString::fromStdString(e.what()));
        msgdlg.exec();
        return;
    }
    catch (...) {
        logger(kipl::logging::Logger::LogError,"Unknown error");
        msgdlg.setDetailedText("An unexpected exeption was thrown");
        msgdlg.exec();
        exit(-1);
    }

    ShowResults();
}

void nGIMainWindow::ShowResults()
{
    std::cout<<"show\n";
    QMessageBox msgdlg;
    msgdlg.setText("An error occurred");

    kipl::base::TImage<float> trans;
    kipl::base::TImage<float> phase;
    kipl::base::TImage<float> dark;
    kipl::base::TImage<float> vis;

    try {
        m_pEngine->GetResults(trans,phase,dark,vis);
    }
    catch (nGIException  &e) {
        logger(kipl::logging::Logger::LogError,e.what());
        msgdlg.setDetailedText(QString::fromStdString(e.what()));
        msgdlg.exec();
        return;
    }
    catch (ModuleException  &e) {
            logger(kipl::logging::Logger::LogError,e.what());
            msgdlg.setDetailedText(QString::fromStdString(e.what()));
            msgdlg.exec();
            return;
        }
    catch (std::exception & e) {
        logger(kipl::logging::Logger::LogError,e.what());
        msgdlg.setDetailedText(QString::fromStdString(e.what()));
        msgdlg.exec();
        return;
    }
    catch (...) {
        logger(kipl::logging::Logger::LogError,"Unknown error");
        msgdlg.setDetailedText("An unexpected exception was thrown.");
        msgdlg.exec();
        exit(-1);
    }

    ui->imageTransmission->set_image(trans.GetDataPtr(),trans.Dims());
    ui->imageDPC->set_image(phase.GetDataPtr(),phase.Dims());
    ui->imageDFI->set_image(dark.GetDataPtr(),dark.Dims());

    const size_t nBins=2048;
    float axis[nBins];
    size_t hist[nBins];
    kipl::base::TImage<float,3> &proj=m_pEngine->GetProjections();
    size_t nLo=0,nHi=0;

    ui->sliderProjections->setRange(m_Config.projections.nFirstIndex,m_Config.projections.nFirstIndex+m_Config.projections.nPhaseSteps);
    //scale_projections->get_adjustment()->configure(0.0, 1.0, mConfig.projections.nPhaseSteps, 1.0, 1.0, 0.0);

    // Plot oscillations
    float proj_osc[2048];
    float ref_osc[2048];

    m_pEngine->OscillationPlot(axis,proj_osc,ref_osc);
 //   ui->plotOscillation->setCurveData(0,axis,proj_osc,m_Config.projections.nPhaseSteps, QColor("blue"),0,QtAddons::PlotGlyph_Square,"Sample");
 //   ui->plotOscillation->plot(axis,ref_osc,m_Config.projections.nPhaseSteps, QColor("green"),1,QtAddons::PlotGlyph_Square,"Reference");
    ui->plotOscillation->setCurveData(0,axis,proj_osc,m_Config.projections.nPhaseSteps);
    ui->plotOscillation->setCurveData(1,axis,ref_osc,m_Config.projections.nPhaseSteps);

    QRect rect;
    if (m_Config.projections.bUseROI==true) {
        if (!ui->checkPreviewCropped->checkState()) {
            rect.setRect(m_Config.projections.nROI[0],
                    m_Config.projections.nROI[1],
                    m_Config.projections.nROI[2]-m_Config.projections.nROI[0],
                    m_Config.projections.nROI[3]-m_Config.projections.nROI[1]);

            ui->imageTransmission->set_rectangle(rect,QColor("yellow"),0);
            ui->imageDPC->set_rectangle(rect,QColor("yellow"),0);
            ui->imageDFI->set_rectangle(rect,QColor("yellow"),0);
        }
        else {
            ui->imageTransmission->clear_rectangle(0);
            ui->imageDPC->clear_rectangle(0);
            ui->imageDFI->clear_rectangle(0);
        }
    }
    else {
        ui->imageTransmission->clear_rectangle(0);
        ui->imageDPC->clear_rectangle(0);
        ui->imageDFI->clear_rectangle(0);
    }

    //on_changed_normroi();
    ui->imageVisibility->set_image(vis.GetDataPtr(),vis.Dims());

    if (m_Config.process.nVisibilityWindowPos[0]*m_Config.process.nVisibilityWindowPos[1]==0) {
        m_Config.process.nVisibilityWindowPos[0]=vis.Size(0)/2;
        m_Config.process.nVisibilityWindowPos[1]=vis.Size(1)/2;
        ui->spinWindowX->setValue(m_Config.process.nVisibilityWindowPos[0]);
        ui->spinWindowY->setValue(m_Config.process.nVisibilityWindowPos[1]);
    }
    else {
        //Draw_VisibilityWindow();
    }
}

void nGIMainWindow::on_checkClampTransmission_toggled(bool checked)
{
    ui->dspinClampTransHigh->setEnabled(checked);
    ui->dspinClampTransLow->setEnabled(checked);
    ui->labelClampTransHigh->setEnabled(checked);
    ui->labelClampTransLow->setEnabled(checked);
}

void nGIMainWindow::on_checkClampDFI_toggled(bool checked)
{
    ui->dspinClampDFIHigh->setEnabled(checked);
    ui->dspinClampDFILow->setEnabled(checked);
    ui->labelClampDFIHigh->setEnabled(checked);
    ui->labelClampDFILow->setEnabled(checked);
}

void nGIMainWindow::UpdateConfig()
{
    QDateTime datetime;
    m_Config.UserInformation.sDate       = datetime.toString().toStdString();
    m_Config.UserInformation.sComment    = ui->textComment->toPlainText().toStdString();
    m_Config.UserInformation.sInstrument = ui->editInstrument->text().toStdString();
    m_Config.UserInformation.sOperator   = ui->editOperator->text().toStdString();
    m_Config.UserInformation.sProjectNumber = ui->editProjectName->text().toStdString();
    m_Config.UserInformation.sSample     = ui->editSample->text().toStdString();

    m_Config.projections.sProjectionMask = ui->editProjectionMask->text().toStdString();
    m_Config.projections.sReferenceMask  = ui->editReferenceMask->text().toStdString();
    m_Config.projections.sDarkMask       = ui->editDarkMask->text().toStdString();
    m_Config.projections.nDarkCnt        = ui->spinDarkCount->value();

    m_Config.projections.sDestPath       = ui->editDestinationPath->text().toStdString();
    m_Config.projections.sDestMask       = ui->editDestinationMask->text().toStdString();

    m_Config.projections.nFilesCnt       = ui->spinImageCount->value();
    m_Config.projections.nPhaseSteps     = ui->spinPhaseSteps->value();
    m_Config.projections.nFileStride     = ui->spinProjectionStride->value();
    m_Config.projections.nFirstIndex     = ui->spinProjectionFirst->value();

    m_Config.projections.nRefFileStride  = ui->spinReferenceStride->value();
    m_Config.projections.nRefFirstIndex  = ui->spinReferenceFirst->value();
    m_Config.projections.nDarkFirstIndex = ui->spinDarkFirst->value();
    m_Config.projections.bCompletePeriod = ui->checkCompletePeriod->checkState();
    m_Config.projections.fPeriods        = ui->spinPeriods->value();
    m_Config.projections.bUseROI         = ui->checkCropImages->checkState();

    m_Config.projections.nROI[0]         = ui->spinCrop0->value();
    m_Config.projections.nROI[1]         = ui->spinCrop1->value();
    m_Config.projections.nROI[2]         = ui->spinCrop2->value();
    m_Config.projections.nROI[3]         = ui->spinCrop3->value();

    m_Config.projections.bUseNorm        = ui->checkDoseRegion->checkState();
    m_Config.projections.nNormROI[0]     = ui->spinDose0->value();
    m_Config.projections.nNormROI[1]     = ui->spinDose1->value();
    m_Config.projections.nNormROI[2]     = ui->spinDose2->value();
    m_Config.projections.nNormROI[3]     = ui->spinDose3->value();

    m_Config.process.bComputeAmplitude   = ui->checkTransmission->checkState();
    m_Config.process.bComputeDFI         = ui->checkDarkField->checkState();
    m_Config.process.bComputeDPC         = ui->checkDPC->checkState();
    m_Config.process.bComputeVisibilty   = true; // needed?
    m_Config.process.bSerialize          = false;
    m_Config.process.bUseAmplLimits      = ui->checkClampTransmission->checkState();
    m_Config.process.bUseDFILimits       = ui->checkClampDFI->checkState();

    m_Config.process.nVisibilityWindow   = ui->spinWindowSize->value();
    m_Config.process.nVisibilityWindowPos[0] = ui->spinWindowX->value();
    m_Config.process.nVisibilityWindowPos[1] = ui->spinWindowY->value();
    m_Config.estimator = ui->ModuleConfEstimator->GetModule();
    m_Config.modules   = ui->ModuleConfPreproc->GetModules();
}

void nGIMainWindow::UpdateDialog()
{
    ui->textComment->setText(QString::fromStdString(m_Config.UserInformation.sComment));
    ui->editInstrument->setText(QString::fromStdString(m_Config.UserInformation.sInstrument));

    ui->editOperator->setText(QString::fromStdString(m_Config.UserInformation.sOperator));
    ui->editProjectName->setText(QString::fromStdString(m_Config.UserInformation.sProjectNumber));
    ui->editSample->setText(QString::fromStdString(m_Config.UserInformation.sSample));

    ui->editProjectionMask->setText(QString::fromStdString(m_Config.projections.sProjectionMask));
    ui->editReferenceMask->setText(QString::fromStdString(m_Config.projections.sReferenceMask));
    ui->editDarkMask->setText(QString::fromStdString(m_Config.projections.sDarkMask));
    ui->spinDarkCount->setValue(m_Config.projections.nDarkCnt);

    ui->editDestinationPath->setText(QString::fromStdString(m_Config.projections.sDestPath));
    ui->editDestinationMask->setText(QString::fromStdString(m_Config.projections.sDestMask));

    ui->spinImageCount->setValue(m_Config.projections.nFilesCnt);
    ui->spinPhaseSteps->setValue(m_Config.projections.nPhaseSteps);
    ui->spinProjectionStride->setValue(m_Config.projections.nFileStride);
    ui->spinProjectionFirst->setValue(m_Config.projections.nFirstIndex);

    ui->spinReferenceStride->setValue(m_Config.projections.nRefFileStride);
    ui->spinReferenceFirst->setValue(m_Config.projections.nRefFirstIndex);
    ui->spinDarkFirst->setValue(m_Config.projections.nDarkFirstIndex);
    ui->checkCompletePeriod->setChecked(m_Config.projections.bCompletePeriod);
    ui->spinPeriods->setValue(m_Config.projections.fPeriods);
    ui->checkCropImages->setChecked(m_Config.projections.bUseROI);

    ui->spinCrop0->setValue(m_Config.projections.nROI[0]);
    ui->spinCrop1->setValue(m_Config.projections.nROI[1]);
    ui->spinCrop2->setValue(m_Config.projections.nROI[2]);
    ui->spinCrop3->setValue(m_Config.projections.nROI[3]);

    ui->checkDoseRegion->setChecked(m_Config.projections.bUseNorm);
    ui->spinDose0->setValue(m_Config.projections.nNormROI[0]);
    ui->spinDose1->setValue(m_Config.projections.nNormROI[1]);
    ui->spinDose2->setValue(m_Config.projections.nNormROI[2]);
    ui->spinDose3->setValue(m_Config.projections.nNormROI[3]);

    ui->checkTransmission->setChecked(m_Config.process.bComputeAmplitude);
    ui->checkDarkField->setChecked(m_Config.process.bComputeDFI);
    ui->checkDPC->setChecked(m_Config.process.bComputeDPC);
    m_Config.process.bComputeVisibilty   = true; // needed?
    m_Config.process.bSerialize          = false;
    ui->checkClampTransmission->setChecked(m_Config.process.bUseAmplLimits);
    ui->checkClampDFI->setChecked(m_Config.process.bUseDFILimits);

    ui->spinWindowSize->setValue(m_Config.process.nVisibilityWindow);
    ui->spinWindowX->setValue(m_Config.process.nVisibilityWindowPos[0]);
    ui->spinWindowY->setValue(m_Config.process.nVisibilityWindowPos[1]);
    ui->ModuleConfEstimator->SetModule(m_Config.estimator);
    ui->ModuleConfPreproc->SetModules(m_Config.modules);
}

void nGIMainWindow::SaveCurrentSetup()
{
    logger(kipl::logging::Logger::LogMessage,"Saving current setup");
    QDir dir;
    QString qcurrentname=dir.homePath()+"/.imagingtools";

    if (!dir.exists(qcurrentname)) {
        dir.mkdir(qcurrentname);
    }
    qcurrentname+="/CurrentnGI.xml";
    std::string currentname=qcurrentname.toStdString();
    kipl::strings::filenames::CheckPathSlashes(currentname,false);
    std::ofstream outfile(currentname.c_str());
    outfile<<m_Config.WriteXML();
}
