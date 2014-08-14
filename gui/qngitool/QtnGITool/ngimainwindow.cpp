#include <QFileDialog>
#include "ngimainwindow.h"
#include "ui_ngimainwindow.h"
#include <io/DirAnalyzer.h>

nGIMainWindow::nGIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::nGIMainWindow)
{
    ui->setupUi(this);
    on_checkCropImages_toggled(ui->checkCropImages->checkState());
    on_checkDoseRegion_toggled(ui->checkDoseRegion->checkState());
    on_checkClampTransmission_toggled(ui->checkClampTransmission->checkState());
    on_checkClampDFI_toggled(ui->checkClampDFI->checkState());
}

nGIMainWindow::~nGIMainWindow()
{
    delete ui;
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

}

void nGIMainWindow::on_buttonProcessAll_clicked()
{

}

void nGIMainWindow::on_buttonPreview_clicked()
{

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
    m_Config.projections.sProjectionMask = ui->editProjectionMask->text().toStdString();
    m_Config.projections.sReferenceMask  = ui->editReferenceMask->text().toStdString();
    m_Config.projections.sDarkMask       = ui->editDarkMask->text().toStdString();
    m_Config.projections.nDarkCnt        = ui->spinDarkCount->value();

    m_Config.projections.sDestPath       = ui->editDestinationPath->text().toStdString();
    m_Config.projections.sDestMask       = ui->editDestinationMask->text().toStdString();

    m_Config.projections.nFilesCnt       = 1;
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
    m_Config.projections.sProjectionMask = ui->editProjectionMask->text().toStdString();
    m_Config.projections.sReferenceMask  = ui->editReferenceMask->text().toStdString();
    m_Config.projections.sDarkMask       = ui->editDarkMask->text().toStdString();
    m_Config.projections.nDarkCnt        = ui->spinDarkCount->value();

    m_Config.projections.sDestPath       = ui->editDestinationPath->text().toStdString();
    m_Config.projections.sDestMask       = ui->editDestinationMask->text().toStdString();

    m_Config.projections.nFilesCnt       = 1;
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
