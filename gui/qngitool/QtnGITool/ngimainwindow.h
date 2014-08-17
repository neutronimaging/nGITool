#ifndef NGIMAINWINDOW_H
#define NGIMAINWINDOW_H

#include <QMainWindow>
#include <nGIConfig.h>
#include <logging/logger.h>

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

private:
    Ui::nGIMainWindow *ui;

    QRect m_CurrentCropROI;
    QRect m_CurrentDoseROI;

    nGIConfig m_Config;

    void UpdateConfig();
    void UpdateDialog();
};

#endif // NGIMAINWINDOW_H
