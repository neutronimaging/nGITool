#include "ngireport.h"
#include <generators/Sine2D.h>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QDir>
#include <QMessageBox>

#include <math/mathconstants.h>

void test_nGIReport()
{
    nGIConfig config;
    nGIReport report;

    size_t imgdims[]={100,50};

    kipl::base::TImage<float,2> img=kipl::generators::Sine2D::SineRings(imgdims,0.5f);

    const size_t N=256;
    float axis[N];
    float proj_osc[N];
    float ref_osc[N];
    config.projections.nPhaseSteps=9;
    float scale = 2*fPi/config.projections.nPhaseSteps;
    for (int i=0; i<config.projections.nPhaseSteps; i++) {
        axis[i]=i*scale;
        proj_osc[i]=cos(axis[i]);
        ref_osc[i]=sin(axis[i]);
    }

    QString reportname=QDir::homePath()+"/testreport.pdf";

    report.CreateReport(reportname, "test.xml",&config,img,img,img,0.0,axis,proj_osc,ref_osc);
}

nGIReport::nGIReport() :
    ReportGeneratorBase("nGIreport")
{
}

int nGIReport::CreateReport(QString filename, std::string projname, nGIConfig *config,
                    kipl::base::TImage<float,2> ampl,
                    kipl::base::TImage<float,2> dpc,
                    kipl::base::TImage<float,2> dfi,
                    float visibility,
                    float *axis,
                    float *ref_osc,
                    float *sample_osc)
{

#ifndef QT_NO_PRINTER
    ostringstream msg;
    OpenDestination(filename);
    m_Painter.begin(&m_Printer);

    MakeHeader(projname);
    m_Painter.setPen(QColor(0,0,0));
    msg.str(""); msg<<"Operator:   "<<config->UserInformation.sOperator;
    Print(msg.str());
    msg.str(""); msg<<"Instrument: "<<config->UserInformation.sInstrument;
    Print(msg.str());
    msg.str(""); msg<<"Sample:     "<<config->UserInformation.sSample;
    Print(msg.str());

    m_fLine+=0.5*m_fLineHeight;
    msg.str(""); msg<<"Config file: "<<projname;
    Print(msg.str());

    m_fLine+=m_fLineHeight;
    Print("Input files",11.0);

    msg.str(""); msg<<"Phase step images: "<<(config->projections.sProjectionMask) ;
    Print(msg.str());

    msg.str(""); msg<<"Open beam: "<<(config->projections.sReferenceMask) ;
    Print(msg.str());

    msg.str(""); msg<<"Dark current: "<<(config->projections.sDarkMask) ;
    Print(msg.str());

    m_fLine+=m_fLineHeight;

    Print("Data configuration",11.0);
    // Modules
    msg.str(""); msg<<"Processed "<<(config->projections.nPhaseSteps)<<" phase steps over "
                    <<(config->projections.fPeriods) <<(1<config->projections.fPeriods ? " periods " : " period ")
                    <<"with "<<(config->projections.bCompletePeriod ? "a complete": "an incomplete")<<" period.";
    Print(msg.str());
    msg.str(""); msg<<"Dose correction activated: "<<(config->projections.bUseNorm ? "yes" : "no");
    Print(msg.str());

    if (config->process.bUseAmplLimits==true) {
        msg.str(""); msg<<"Clamping transmission image to interval ["<<config->process.fAmplLimits[0]<<", "<<config->process.fAmplLimits[1]<<"]";
        Print(msg.str());
    }

    if (config->process.bUseDFILimits==true) {
        msg.str(""); msg<<"Clamped dark field image to interval ["<<config->process.fDFILimits[0]<<", "<<config->process.fDFILimits[1]<<"]";
        Print(msg.str());
    }
    m_fLine+=m_fLineHeight;

    ProcessingModules(config);

    int dstdims[]={0.88*m_fWidth/3,0.88*m_fWidth/3};
    m_fLine+=m_fLineHeight;
    Print("Amplitude",0,m_fLine,11);
    Print("DPC",m_fWidth/3,m_fLine,11);
    Print("DFI",2*m_fWidth/3,m_fLine,11);


    m_ImagePainter.set_image(ampl.GetDataPtr(),ampl.Dims());
    m_ImagePainter.Render(m_Painter,0,m_fLine,dstdims[0],dstdims[1]);

    m_ImagePainter.set_image(dpc.GetDataPtr(),dpc.Dims());
    m_ImagePainter.Render(m_Painter,m_fWidth/3,static_cast<int>(m_fLine),dstdims[0],dstdims[1]);

    m_ImagePainter.set_image(dfi.GetDataPtr(),dfi.Dims());
    m_ImagePainter.Render(m_Painter,2*(m_fWidth/3),static_cast<int>(m_fLine),dstdims[0],dstdims[1]);

    m_fLine+=dstdims[1]+m_fLineHeight;
    msg.str(""); msg<<"Visibility: "<<visibility;
    Print(msg.str());

    Print("Oscillation plot",0.6*m_fWidth,m_fLine,11);
    m_fLine+=0.75*m_fLineHeight;
    m_OscillationPlot.setCurveData(0,axis,ref_osc,config->projections.nPhaseSteps);
    m_OscillationPlot.setCurveData(1,axis,sample_osc,config->projections.nPhaseSteps);
    m_OscillationPlot.m_bShowGrid=true;

    //m_OscillationPlot.ShowAxes(true);
    //m_OscillationPlot.ShowLegend(true);
    m_OscillationPlot.Render(m_Painter,0.6*m_fWidth,m_fLine,0.4*m_fWidth,0.3*m_fWidth);

    m_fLine+=m_fLineHeight;

    if (!config->UserInformation.sComment.empty()) {
        Print("Comment",11.0);
        Print(config->UserInformation.sComment);
    }

    m_Painter.end();

    msg.str(""); msg<< "Wrote PDF file \"" << filename.toStdString() << "\"";
    logger(kipl::logging::Logger::LogVerbose,msg.str());
#else
    logger(kipl::logging::Logger::LogMessage,"Printing is disabled");
//    QMessageBox msgbox(Icon(),QString("No printing"),QString("Printing is currently disabled."),QMessageBox::StandardButton::Ok,NULL,);
//    msgbox.exec();
#endif
    return 0;
}


void nGIReport::ProcessingModules(nGIConfig *config)
{
    ostringstream msg;
    msg.str("");

    Print("Preprocessing modules:",11.0);
    if (config->modules.empty()) {
        Print("No pre-processing modules were used.");
    }
    else {
        msg<<"Used "<<config->modules.size()<<" pre processing modules";
        Print(msg.str());
        std::list<ModuleConfig>::iterator it;
        for (it=config->modules.begin(); it!=config->modules.end(); it++) {
            if (it->m_bActive) {
                msg.str("");
                msg<<it->m_sModule<<" ["<<it->m_sSharedObject<<"]";
                Print(msg.str());
            }
        }
    }
    m_fLine+=m_fLineHeight;
    Print("Estimator",11.0);
    msg.str("");
    msg<<config->estimator.m_sModule<<" ["<<config->estimator.m_sSharedObject<<"]";
    Print(msg.str());

    m_fLine+=2*m_fLineHeight;
}
