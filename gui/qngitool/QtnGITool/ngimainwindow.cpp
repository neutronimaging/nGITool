#include "ngimainwindow.h"
#include "ui_ngimainwindow.h"

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

}

void nGIMainWindow::on_buttonReferenceBrowse_clicked()
{

}

void nGIMainWindow::on_buttonDarkBrowse_clicked()
{

}

void nGIMainWindow::on_buttonDestinationBrowse_clicked()
{

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
