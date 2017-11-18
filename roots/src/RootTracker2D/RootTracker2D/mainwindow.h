#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RootTrackConfig.h"
#include <base/timage.h>
#include <logging/logger.h>

#include "rootfinder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    kipl::logging::Logger logger;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_button_browsefile_clicked();

    void on_button_loadimage_clicked();

    void on_button_process_clicked();

    void on_combo_displayimage_currentIndexChanged(const QString &arg1);

    void on_button_getroi_clicked();

    void on_slider_kernels_sliderMoved(int position);

    void on_button_saveimages_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionNew_triggered();

    void on_actionQuit_triggered();

private:
    void UpdateConfig();
    void UpdateDialog();
    void SaveConfiguration(QString qfname);
    void LoadDefaults();
    void UpdateSaveList();

    Ui::MainWindow *ui;
    std::string m_sParameterFileName;
    RootTrackConfig m_Config;
    RootFinder m_RootFinder;

    kipl::base::TImage<float,2> m_OriginalImage;
    kipl::base::TImage<float,2> m_ResultImage;
};

#endif // MAINWINDOW_H
