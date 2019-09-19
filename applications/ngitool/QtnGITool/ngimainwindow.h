#ifndef NGIMAINWINDOW_H
#define NGIMAINWINDOW_H

#include <QMainWindow>
#include <nGIConfig.h>
#include <logging/logger.h>
#include <nGIEngine.h>

namespace Ui {
class nGIMainWindow;
}

class nGIMainWindow : public QMainWindow
{
    Q_OBJECT
    kipl::logging::Logger logger;
public:
    explicit nGIMainWindow(QApplication *app, QWidget *parent = 0);
    QApplication *m_QtApp;
    ~nGIMainWindow();
    
private slots:


    void on_checkCropImages_toggled(bool checked);

    void on_buttonGetCropROI_clicked();

    void on_buttonGetDoseROI_clicked();

    void on_checkDoseRegion_toggled(bool checked);

    void on_buttonBrowseProjection_clicked();

    void on_buttonReferenceBrowse_clicked();

    void on_buttonDarkBrowse_clicked();

    void on_buttonDestinationBrowse_clicked();

    void on_buttonCreateReport_clicked();

    void on_buttonProcessAll_clicked();

    void on_buttonPreview_clicked();

    void on_checkClampTransmission_toggled(bool checked);

    void on_checkClampDFI_toggled(bool checked);

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionQuit_triggered();
    void on_actionPrint_triggered();

    void on_buttonGetVisROI_clicked();
    void on_spinVisROI0_valueChanged(int arg1);
    void on_spinVisROI1_valueChanged(int arg1);
    void on_spinVisROI2_valueChanged(int arg1);
    void on_spinVisROI3_valueChanged(int arg1);

    void on_spinCrop0_valueChanged(int arg1);
    void on_spinCrop1_valueChanged(int arg1);
    void on_spinCrop2_valueChanged(int arg1);
    void on_spinCrop3_valueChanged(int arg1);

    void on_spinDose0_valueChanged(int arg1);
    void on_spinDose1_valueChanged(int arg1);
    void on_spinDose2_valueChanged(int arg1);
    void on_spinDose3_valueChanged(int arg1);

    void on_sliderProjections_valueChanged(int value);

private:
    Ui::nGIMainWindow *ui;

    QRect m_CurrentCropROI;
    QRect m_CurrentDoseROI;

    std::string m_sConfigFilename;
    std::string m_sApplicationName;

    float m_fVisibility;
    bool m_bNewResult;

    nGIConfig m_Config;
    nGIEngine *m_pEngine;
    void LoadDefaults();
    void SaveCurrentSetup();

    void ShowResults();
    void Draw_VisibilityWindow();
    void DisplayNewProjection(int slice);
    void UpdateConfig();
    void UpdateDialog();

    void on_spinVisROI_changed(int x);
    void on_spinCrop_Changed(int x);
    void on_spinDose_Changed(int x);
};

#endif // NGIMAINWINDOW_H
