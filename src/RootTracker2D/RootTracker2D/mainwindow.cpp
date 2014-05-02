#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <io/io_tiff.h>
#include <base/KiplException.h>
#include <base/thistogram.h>
#include <ModuleException.h>
#include <sstream>
#include <QListWidgetItem>

#include "rootfinder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    logger("MainWindow"),
    ui(new Ui::MainWindow),
    m_sParameterFileName("noname.xml")
{
    setWindowTitle("RootTracker2D");
    ui->setupUi(this);
    logger.AddLogTarget(*ui->loggingview);
    QDir dirs;

    if (!dirs.exists(dirs.homePath()+"/.imagingtools")) {
        logger(kipl::logging::Logger::LogMessage,"Created dir .imagingtools in your home dir.");
        dirs.mkdir(dirs.homePath()+"/.imagingtools");
    }

    LoadDefaults();

    UpdateDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadDefaults()
{
    std::string defaultsname;
    QDir dir;
    QString currentname=QDir::homePath()+"/.imagingtools/CurrentRootTrackConfig.xml";

    bool bUseDefaults=true;
    if (dir.exists(currentname)) {
        defaultsname=currentname.toStdString();
        bUseDefaults=false;
    }

    std::ostringstream msg;

    kipl::strings::filenames::CheckPathSlashes(defaultsname,false);
    try {
        m_Config.LoadConfigFile(defaultsname.c_str(),"roottracker");
        logger(kipl::logging::Logger::LogMessage,"Loaded the settings from last session.");
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
        m_Config.mImageInformation.filename = QDir::homePath().toStdString() + "/image.tif";
    }
    UpdateDialog();
    on_button_loadimage_clicked();
}

void MainWindow::on_button_browsefile_clicked()
{
    QString fname=QFileDialog::getOpenFileName(this,
                                      "Select file to open",
                                      QDir::currentPath());
   ui->edit_filename->setText(fname);

   on_button_loadimage_clicked();
}

void MainWindow::on_button_loadimage_clicked()
{
    UpdateConfig();
    QDir dir;
    if (!dir.exists(QString::fromStdString(m_Config.mImageInformation.filename))) {
        logger(kipl::logging::Logger::LogError,"Image file does not exist");
        return;
    }
    try {
    if (m_Config.mImageInformation.useroi)
        kipl::io::ReadTIFF(m_OriginalImage,m_Config.mImageInformation.filename.c_str(),m_Config.mImageInformation.img_roi);
    else
        kipl::io::ReadTIFF(m_OriginalImage,m_Config.mImageInformation.filename.c_str(),NULL);
    }
    catch (kipl::base::KiplException &e) {
        std::stringstream msg;

        msg<<"Load failed: "<<e.what();
        logger(kipl::logging::Logger::LogError,msg.str());
        QMessageBox mbox;
        mbox.setText(QString::fromStdString(msg.str()));
        mbox.exec();
        return;
    }

    kipl::strings::filenames::StripFileName(m_Config.mImageInformation.filename,
                                            m_Config.mImageInformation.path,
                                            m_Config.mImageInformation.basename,
                                            m_Config.mImageInformation.extensions);

    ui->combo_displayimage->clear();
    ui->combo_displayimage->addItem("Original");
    ui->combo_displayimage->setCurrentIndex(0);
    ui->viewer_images->set_image(m_OriginalImage.GetDataPtr(),m_OriginalImage.Dims());
}

void MainWindow::on_button_process_clicked()
{
    UpdateConfig();
    QString qfname=QDir::homePath()+"/.imagingtools/CurrentRootTrackConfig.xml";
    SaveConfiguration(qfname);
    QMessageBox mbox;
    if (m_OriginalImage.Size()==0) {
        logger(kipl::logging::Logger::LogError,"Please load an image before processing.");

        mbox.setText(QString("Please, load an image before processing."));
        mbox.exec();
        return;
    }


    try {
        m_RootFinder.Process(m_OriginalImage,m_ResultImage,m_Config);
    }
    catch (kipl::base::KiplException &e) {
        mbox.setText(QString::fromStdString(e.what()));
        mbox.exec();
        return;
    }
    catch (std::exception &e) {
        mbox.setText(QString::fromStdString(e.what()));
        mbox.exec();
        return;
    }

    ui->combo_displayimage->clear();
    std::map<std::string, kipl::base::TImage<float,2> >::iterator it;
    int i=0;
    cout<<m_RootFinder.m_ImageList.size()<<endl;
    for (it=m_RootFinder.m_ImageList.begin(); it!=m_RootFinder.m_ImageList.end(); it++,i++) {
        cout<<i<<" "<<it->first<<endl;
        ui->combo_displayimage->addItem(QString::fromStdString(it->first));
    }
    ui->combo_displayimage->setCurrentIndex(i-1);
    ui->slider_kernels->setRange(0,m_RootFinder.m_KernelList.size()-1);
    UpdateSaveList();
}

void MainWindow::UpdateConfig()
{
    m_Config.mImageInformation.filename = ui->edit_filename->text().toStdString();

    m_Config.mImageInformation.useroi = ui->check_useroi->checkState();
    m_Config.mImageInformation.img_roi[0] = ui->spin_roi0->value();
    m_Config.mImageInformation.img_roi[1] = ui->spin_roi1->value();
    m_Config.mImageInformation.img_roi[2] = ui->spin_roi2->value();
    m_Config.mImageInformation.img_roi[3] = ui->spin_roi3->value();

    //m_Config.m_nSmallItem = ui->spin_
    m_Config.mProcessingSettings.thres_quantile = ui->dspin_thresquantile->value();
    m_Config.mProcessingSettings.thres_clean = ui->spin_thresclean->value();
    m_Config.mProcessingSettings.thres_low = ui->dspin_threslow->value();
    m_Config.mProcessingSettings.m_nHistogramSize = ui->spin_histsize->value();

    m_Config.mProcessingSettings.mfr_sigma = ui->dspin_mfrsigma->value();
    m_Config.mProcessingSettings.mfr_ratio = ui->dspin_mfrratio->value();
    m_Config.mProcessingSettings.mfr_dirs  = ui->spin_mfrdir->value();
    m_Config.mProcessingSettings.mfr_omega = ui->dspin_mfromega->value();

    m_Config.mProcessingSettings.m_bTracking = ui->check_tracking->checkState();

    m_Config.mProcessingSettings.piece_fringe   = ui->dspin_growfringe->value();
    m_Config.mProcessingSettings.piece_maxsize  = ui->spin_piecemax->value();
    m_Config.mProcessingSettings.piece_minsize  = ui->spin_piecemin->value();
    m_Config.mProcessingSettings.piece_testsize = ui->spin_piecetest->value();
    m_Config.mProcessingSettings.grow_thresdec  = ui->dspin_growthresdec->value();
    m_Config.mProcessingSettings.grow_thresmin  = ui->dspin_growthres->value();
    m_Config.mProcessingSettings.piece_tree     = ui->dspin_growtree->value();

    m_Config.mProcessingSettings.sample_thick = ui->dspin_samplethickness->value();
    //m_Config.sample_volume = ui->spin_
    m_Config.mProcessingSettings.pixel_size = ui->dspin_pixelsize->value();
    m_Config.mProcessingSettings.pixel_unit = ui->edit_pixelunit->text().toStdString();
}

void MainWindow::UpdateDialog()
{
    ui->edit_filename->setText(QString::fromStdString(m_Config.mImageInformation.filename));
    ui->spin_roi0->setValue(m_Config.mImageInformation.img_roi[0]);
    ui->spin_roi1->setValue(m_Config.mImageInformation.img_roi[1]);
    ui->spin_roi2->setValue(m_Config.mImageInformation.img_roi[2]);
    ui->spin_roi3->setValue(m_Config.mImageInformation.img_roi[3]);
    ui->check_useroi->setChecked(m_Config.mImageInformation.useroi);

    //int img_rot;
    //int img_log;

    //int m_nSmallItem;
    ui->spin_thresclean->setValue(m_Config.mProcessingSettings.thres_clean);
    ui->dspin_growthres->setValue(m_Config.mProcessingSettings.thres_low);
    ui->dspin_thresquantile->setValue(m_Config.mProcessingSettings.thres_quantile);
    ui->spin_histsize->setValue(m_Config.mProcessingSettings.m_nHistogramSize);

    ui->dspin_mfrsigma->setValue(m_Config.mProcessingSettings.mfr_sigma);
    ui->spin_mfrdir->setValue(m_Config.mProcessingSettings.mfr_dirs);
    ui->dspin_mfrratio->setValue(m_Config.mProcessingSettings.mfr_ratio);
    ui->dspin_mfromega->setValue(m_Config.mProcessingSettings.mfr_omega);
    ui->check_tracking->setChecked(m_Config.mProcessingSettings.m_bTracking);
    ui->spin_piecemax->setValue(m_Config.mProcessingSettings.piece_maxsize);
    ui->spin_piecemin->setValue(m_Config.mProcessingSettings.piece_minsize);
    ui->spin_piecetest->setValue(m_Config.mProcessingSettings.piece_testsize);
    ui->dspin_growthres->setValue(m_Config.mProcessingSettings.grow_thresmin);
    ui->dspin_growthresdec->setValue(m_Config.mProcessingSettings.grow_thresdec);
    ui->dspin_growfringe->setValue(m_Config.mProcessingSettings.piece_fringe);
    ui->dspin_growtree->setValue(m_Config.mProcessingSettings.piece_tree);

    ui->dspin_samplethickness->setValue(m_Config.mProcessingSettings.sample_thick);

//    float sample_volume;
//    int save_steps;
//    int vol_est;
    ui->dspin_pixelsize->setValue(m_Config.mProcessingSettings.pixel_size);
    ui->edit_pixelunit->setText(QString::fromStdString(m_Config.mProcessingSettings.pixel_unit));

}

void MainWindow::SaveConfiguration(QString qfname)
{
    std::string fname = qfname.toStdString();
    kipl::strings::filenames::CheckPathSlashes(fname,false);
    std::ofstream cfgfile(fname.c_str());

    cfgfile<<m_Config.WriteXML();
}

void MainWindow::on_combo_displayimage_currentIndexChanged(const QString &arg1)
{
    if (!m_RootFinder.m_ImageList.empty()) {
        kipl::base::TImage<float,2> & img=m_RootFinder.m_ImageList[arg1.toStdString()];
        const int N=512;
        size_t *hist=new size_t[N];
        float *axis=new float[N];
        kipl::base::Histogram(img.GetDataPtr(), img.Size(), hist, N, 0.0f, 0.0f, axis);
        size_t low  = 0;
        size_t high = 0;

        kipl::base::FindLimits(hist, N, 99.0f, &low, &high);
        ui->viewer_images->set_image(img.GetDataPtr(),img.Dims(),axis[low],axis[high]);
        delete [] hist;
        delete [] axis;
    }
}

void MainWindow::on_button_getroi_clicked()
{
    QRect roi = ui->viewer_images->get_marked_roi();
    int x0;
    int y0;
    int x1;
    int y1;
    roi.getCoords(&x0,&y0, &x1,&y1);
    ui->spin_roi0->setValue(x0);
    ui->spin_roi1->setValue(y0);
    ui->spin_roi2->setValue(x1);
    ui->spin_roi3->setValue(y1);

}

void MainWindow::on_slider_kernels_sliderMoved(int position)
{
    if (!m_RootFinder.m_KernelList.empty()) {
        kipl::base::TImage<float,2> & img= m_RootFinder.m_KernelList[position];
        ui->viewer_kernel->set_image(img.GetDataPtr(),img.Dims());
    }
}

void MainWindow::UpdateSaveList()
{
    ui->list_imagenames->clear();
    std::map<std::string, kipl::base::TImage<float,2> >::iterator it;

    for (it=m_RootFinder.m_ImageList.begin(); it!=m_RootFinder.m_ImageList.end(); it++) {
        QListWidgetItem *item = new QListWidgetItem;

        if (!it->first.empty()) {
            item->setText(QString::fromStdString(it->first));

            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Checked); // AND initialize check state

            ui->list_imagenames->insertItem(ui->list_imagenames->currentRow()+1,item);
            ui->list_imagenames->setCurrentItem(item);
        }
    }
}

void MainWindow::on_button_saveimages_clicked()
{
    std::ostringstream msg;
    QString fname=QFileDialog::getSaveFileName(this,
                                      "Enter file name to save",
                                      QString::fromStdString(m_Config.mImageInformation.path));
                                     // QDir::homePath());

    std::string path,name;
    vector<std::string> ext;

    kipl::strings::filenames::StripFileName(fname.toStdString(),path,name,ext);

    for (int i=0; i<(ui->list_imagenames->count()); i++) {
        QListWidgetItem * item=ui->list_imagenames->item(i);

        if (item->checkState()==Qt::Checked) {
            QString imgname=item->text();
            kipl::base::TImage<float,2> & img = m_RootFinder.m_ImageList[imgname.toStdString()];
            imgname.replace(' ','_');

            std::string fname=path+name+(name[name.size()-1]=='_' ? "":"_")+imgname.toStdString()+".tif";
            msg.str("");
            msg<<"Saving "<<fname;
            logger(kipl::logging::Logger::LogMessage,msg.str());
            kipl::io::WriteTIFF32(img,fname.c_str());
        }
    }
    std::string cfname=path+name+"_config.xml";

    ofstream conf_file(cfname.c_str());

    conf_file<<m_Config.WriteXML();

}


void MainWindow::on_actionOpen_triggered()
{
    QString fname=QFileDialog::getOpenFileName(this,
                                      "Select parameter file to open",
                                      QDir::currentPath());

    QMessageBox box;
    ostringstream msg;
    try {
        m_Config.LoadConfigFile(fname.toStdString(),"roottracker");
        UpdateDialog();
    }
    catch (ModuleException &e) {
        msg.str("");

        msg<<"Failed to open paramter file\n"<<e.what();
        box.setText(QString::fromStdString(msg.str()));
    }
    catch (kipl::base::KiplException &e) {
        msg.str("");

        msg<<"Failed to open paramter file\n"<<e.what();
        box.setText(QString::fromStdString(msg.str()));
    }

    catch (std::exception &e) {
        msg.str("");

        msg<<"Failed to open paramter file\n"<<e.what();
        box.setText(QString::fromStdString(msg.str()));
    }
}


void MainWindow::on_actionSave_triggered()
{

    if (m_sParameterFileName=="noname.xml") {
        on_actionSave_as_triggered();
    }
    else {
        std::ofstream parfile(m_sParameterFileName.c_str());

        UpdateConfig();
        parfile<<m_Config.WriteXML();
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fname=QFileDialog::getSaveFileName(this,
                                      "Select location to save parameters",
                                      QDir::currentPath());

    QMessageBox box;
    ostringstream msg;
    try {
        m_sParameterFileName=fname.toStdString();
        std::ofstream parfile(m_sParameterFileName.c_str());
        UpdateConfig();

        parfile<<m_Config.WriteXML();
    }
    catch (ModuleException &e) {
        msg.str("");

        msg<<"Failed to save parameter file\n"<<e.what();
        box.setText(QString::fromStdString(msg.str()));
    }
    catch (kipl::base::KiplException &e) {
        msg.str("");

        msg<<"Failed to save parameter file\n"<<e.what();
        box.setText(QString::fromStdString(msg.str()));
    }
    catch (std::exception &e) {
        msg.str("");

        msg<<"Failed to save parameter file\n"<<e.what();
        box.setText(QString::fromStdString(msg.str()));
    }
}

void MainWindow::on_actionNew_triggered()
{
    RootTrackConfig conf;
    m_Config=conf;
    UpdateDialog();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}
