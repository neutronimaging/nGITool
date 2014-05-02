#include "ngireport.h"
#include <generators/Sine2D.h>
#include <QPrinter>
#include <QPainter>
#include <QDir>

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



    m_Painter.begin(&m_Printer);


    MakeHeader(projname);
    ostringstream msg;

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

    msg.str(""); msg<<"Phase step images: "<<(config->projections.sPath)<<(config->projections.sFileMask) ;
    Print(msg.str());

    msg.str(""); msg<<"Open beam: "<<(config->projections.sReferencePath)<<(config->projections.sObMask) ;
    Print(msg.str());

    msg.str(""); msg<<"Dark current: "<<(config->projections.sDarkPath)<<(config->projections.sDarkMask) ;
    Print(msg.str());

    m_fLine+=m_fLineHeight;

    Print("Data configuration",11.0);
//    Print(&painter,"Oscillation plot",0.6*width,m_fLine);
//    m_OscillationPlot.SetPlotData(axis,ref_osc,
//            config->projections.nPhaseSteps,
//            Gdk::Color("green"),
//            0,
//            Gtk_addons::PlotGlyph_Square,
//            "Reference");

//    m_OscillationPlot.SetPlotData(axis,sample_osc,
//            config->projections.nPhaseSteps,
//            Gdk::Color("blue"),
//            1,
//            Gtk_addons::PlotGlyph_Square,
//            "Sample");

//    m_OscillationPlot.ShowAxes(true);
//    m_OscillationPlot.ShowLegend(true);
//    m_OscillationPlot.Render(cr,0.6*width, m_fLine+m_fLineHeight, 0.35*width,4*m_fLineHeight);

    m_fLine+=m_fLineHeight;

    if (!config->UserInformation.sComment.empty()) {
        Print("Comment",11.0);
        Print(config->UserInformation.sComment);
    }

    m_Painter.end();



#ifdef CAIRO_HAS_PDF_SURFACE



    msg.str(""); msg<<"Processed "<<(config->projections.nPhaseSteps)<<" phase steps over "
                    <<(config->projections.fPeriods) <<(1<config->projections.fPeriods ? " periods " : " period ")
                    <<"with "<<(config->projections.bCompletePeriod ? "a complete": "an incomplete")<<" period.";
    Print(cr,msg.str());
    msg.str(""); msg<<"Dose correction activated: "<<(config->projections.bUseNorm ? "yes" : "no");
    Print(cr,msg.str());

    if (config->process.bUseAmplLimits==true) {
        msg.str(""); msg<<"Clamping transmission image to interval ["<<config->process.fAmplLimits[0]<<", "<<config->process.fAmplLimits[1]<<"]";
        Print(cr,msg.str());
    }

    if (config->process.bUseDFILimits==true) {
        msg.str(""); msg<<"Clamped dark field image to interval ["<<config->process.fDFILimits[0]<<", "<<config->process.fDFILimits[1]<<"]";
        Print(cr,msg.str());
    }
    m_fLine+=m_fLineHeight;
    cr->stroke();



    ProcessingModules(cr,config);
    // Images
    int dstdims[]={160,160};
    m_fLine+=m_fLineHeight;
    cr->move_to(m_fMargin,m_fLine);
    cr->show_text("Amplitude");
    cr->move_to(m_fMargin+dstdims[0]+20,m_fLine);
    cr->show_text("DPC");
    cr->move_to(m_fMargin+2*(dstdims[0]+20),m_fLine);
    cr->show_text("DFI");

    m_fLine+=0.75*m_fLineHeight;

    m_ImageRenderer.set_image(ampl.GetDataPtr(),ampl.Dims());
    m_ImageRenderer.Render(cr,static_cast<int>(m_fMargin),static_cast<int>(m_fLine),dstdims[0],dstdims[1]);

    m_ImageRenderer.set_image(dpc.GetDataPtr(),dpc.Dims());
    m_ImageRenderer.Render(cr,static_cast<int>(m_fMargin)+(dstdims[0]+20),static_cast<int>(m_fLine),dstdims[0],dstdims[1]);

    m_ImageRenderer.set_image(dfi.GetDataPtr(),dfi.Dims());
    m_ImageRenderer.Render(cr,static_cast<int>(m_fMargin)+2*(dstdims[0]+20),static_cast<int>(m_fLine),dstdims[0],dstdims[1]);

    m_fLine+=dstdims[1]+m_fLineHeight;
    msg.str(""); msg<<"Visibility: "<<visibility;
    Print(cr,msg.str());
    m_fLine+=m_fLineHeight;

    if (!config->UserInformation.sComment.empty()) {
        Print(cr,"Comment",11.0);
        Print(cr,config->UserInformation.sComment);
        cr->stroke();
    }

    cr->show_page();



    return 0;

#endif
    msg.str(""); msg<< "Wrote PDF file \"" << filename.toStdString() << "\"";
    logger(kipl::logging::Logger::LogVerbose,msg.str());
}
