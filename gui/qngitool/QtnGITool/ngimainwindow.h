#ifndef NGIMAINWINDOW_H
#define NGIMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class nGIMainWindow;
}

class nGIMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit nGIMainWindow(QWidget *parent = 0);
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

private:
    Ui::nGIMainWindow *ui;
    QRect m_CurrentCropROI;
    QRect m_CurrentDoseROI;
};

#endif // NGIMAINWINDOW_H
