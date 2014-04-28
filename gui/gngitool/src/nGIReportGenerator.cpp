
#include "stdafx.h"
#include "../include/nGIReportGenerator.h"
#include <nGIConfig.h>

#include <sstream>
#include <string>
#include <iostream>

//#include <cairomm/cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <glibmm/date.h>
#include <glibmm/timeval.h>
#include <cstdlib>

#include <generators/Sine2D.h>
#include <GtkGlyphs.h>
#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif 

#ifdef HAVE_MATH_H
#include <math.h>
#endif

void test_nGIReport()
{

	nGIConfig config;
	int dims[]={595,842};
	nGIReportGenerator report(dims);

	size_t imgdims[]={100,50};

	kipl::base::TImage<float,2> img=kipl::generators::Sine2D::SineRings(imgdims,0.5f);

	const size_t N=256;
	float axis[N];
	float proj_osc[N];
	float ref_osc[N];

	std::string reportname="testreport.pdf";

	report.CreateReport(reportname, "test.xml",&config,img,img,img,0.0,axis,proj_osc,ref_osc);


}


nGIReportGenerator::nGIReportGenerator(int *dims) :
ReportGenerator(dims,"nGIReportGenerator")
{}

int nGIReportGenerator::CreateReport(std::string filename, std::string projname, nGIConfig *config,
					kipl::base::TImage<float,2> ampl,
					kipl::base::TImage<float,2> dpc,
					kipl::base::TImage<float,2> dfi,
					float visibility,
					float *axis,
					float *ref_osc,
					float *sample_osc)
{
#ifdef CAIRO_HAS_PDF_SURFACE
    Cairo::RefPtr<Cairo::PdfSurface> surface =
        Cairo::PdfSurface::create(filename, width, height);

    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    cr->save(); // save the state of the context
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();    // fill image with the color
    cr->restore();  // color is back to black now

    cr->save();
    // draw a border around the image
    //cr->set_line_width(20.0);    // make the line wider
    //cr->rectangle(0.0, 0.0, cairo_image_surface_get_width(surface->cobj()), height);
    //cr->stroke();

    MakeHeader(cr,config->UserInformation.sProjectNumber);
	ostringstream msg;

	msg.str(""); msg<<"Operator:   "<<config->UserInformation.sOperator;
	Print(cr,msg.str());
	msg.str(""); msg<<"Instrument: "<<config->UserInformation.sInstrument;
	Print(cr,msg.str());
	msg.str(""); msg<<"Sample:     "<<config->UserInformation.sSample;
	Print(cr,msg.str());

	m_fLine+=0.5*m_fLineHeight;
	msg.str(""); msg<<"Config file: "<<projname;
	Print(cr,msg.str());

	m_fLine+=m_fLineHeight;
	Print(cr,"Input files",11.0);

	msg.str(""); msg<<"Phase step images: "<<(config->projections.sPath)<<(config->projections.sFileMask) ;
	Print(cr,msg.str());

	msg.str(""); msg<<"Open beam: "<<(config->projections.sReferencePath)<<(config->projections.sObMask) ;
	Print(cr,msg.str());

	msg.str(""); msg<<"Dark current: "<<(config->projections.sDarkPath)<<(config->projections.sDarkMask) ;
	Print(cr,msg.str());

	m_fLine+=m_fLineHeight;

	Print(cr,"Data configuration",11.0);
	Print(cr,"Oscillation plot",0.6*width,m_fLine);
	m_OscillationPlot.SetPlotData(axis,ref_osc,
			config->projections.nPhaseSteps,
			Gdk::Color("green"),
			0,
			Gtk_addons::PlotGlyph_Square,
			"Reference");

	m_OscillationPlot.SetPlotData(axis,sample_osc,
			config->projections.nPhaseSteps,
			Gdk::Color("blue"),
			1,
			Gtk_addons::PlotGlyph_Square,
			"Sample");

	m_OscillationPlot.ShowAxes(true);
	m_OscillationPlot.ShowLegend(true);
	m_OscillationPlot.Render(cr,0.6*width, m_fLine+m_fLineHeight, 0.35*width,4*m_fLineHeight);
	cr->stroke();

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
	msg.str("");

    msg<< "Wrote PDF file \"" << filename << "\"";
	logger(kipl::logging::Logger::LogVerbose,msg.str());

    return 0;

#else

    std::cout << "You must compile cairo with PDF support for this example to work."
        << std::endl;
    return 1;

#endif

}

int nGIReportGenerator::TestReport(std::string filename)
{
#ifdef CAIRO_HAS_PDF_SURFACE
    Cairo::RefPtr<Cairo::PdfSurface> surface =
        Cairo::PdfSurface::create(filename, width, height);

    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    cr->save(); // save the state of the context
    cr->set_source_rgb(0.86, 0.85, 0.47);
    cr->paint();    // fill image with the color
    cr->restore();  // color is back to black now

    cr->save();
    // draw a border around the image
    cr->set_line_width(20.0);    // make the line wider
    cr->rectangle(0.0, 0.0, cairo_image_surface_get_width(surface->cobj()), height);
    cr->stroke();

    cr->set_source_rgba(0.0, 0.0, 0.0, 0.7);
    // draw a circle in the center of the image
    cr->arc(width / 2.0, height / 2.0, 
            height / 4.0, 0.0, 2.0 * 3.1415);
    cr->stroke();

    // draw a diagonal line
    cr->move_to(width / 4.0, height / 4.0);
    cr->line_to(width * 3.0 / 4.0, height * 3.0 / 4.0);
    cr->stroke();
    cr->restore();

    cr->show_page();

    std::cout << "Wrote PDF file \"" << filename << "\"" << std::endl;
    return 0;

#else

    std::cout << "You must compile cairo with PDF support for this example to work."
        << std::endl;
    return 1;

#endif

}

double nGIReportGenerator::MakeHeader(Cairo::RefPtr<Cairo::Context> cr, std::string name)
{
	std::ostringstream msg;

	msg<<"Interferometer results for "<<name;
	ReportGenerator::MakeHeader(cr,msg.str());

	return 0;
}

void nGIReportGenerator::ProcessingModules(Cairo::RefPtr<Cairo::Context> cr,nGIConfig *config)
{
	ostringstream msg;
	msg.str("");

	Print(cr,"Preprocessing modules:",12.0);
	if (config->modules.empty()) {
		Print(cr,"No pre-processing modules were used.");
	}
	else {
		msg<<"Used "<<config->modules.size()<<" pre processing modules";
		Print(cr,msg.str());
		std::list<ModuleConfig>::iterator it;
		for (it=config->modules.begin(); it!=config->modules.end(); it++) {
			if (it->m_bActive) {
				msg.str("");
				msg<<it->m_sModule<<" ["<<it->m_sSharedObject<<"]";
				Print(cr,msg.str());
			}
		}
	}
	m_fLine+=m_fLineHeight;
	Print(cr,"Estimator",12.0);
	msg.str("");
	msg<<config->estimator.m_sModule<<" ["<<config->estimator.m_sSharedObject<<"]";
	Print(cr,msg.str());

	m_fLine+=2*m_fLineHeight;
}
