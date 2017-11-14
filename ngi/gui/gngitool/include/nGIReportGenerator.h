#ifndef __NGIREPORTGENERATOR_H
#define __NGIREPORTGENERATOR_H

#include <string>
#include <sstream>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include <ReportGenerator.h>
#include <ContextPlotter.h>
#include <ContextImageViewer.h>

#include <base/timage.h>

#include <nGIConfig.h>

void test_nGIReport();

class nGIReportGenerator : public Gtk_addons::ReportGenerator
{
public:
	nGIReportGenerator(int *dims);
	int TestReport(std::string filename);
	int CreateReport(std::string filename, std::string projname, nGIConfig *config,
						kipl::base::TImage<float,2> ampl,
						kipl::base::TImage<float,2> dpc,
						kipl::base::TImage<float,2> dfi,
						float visibility,
						float *axis,
						float *ref_osc,
						float *sample_osc);

	virtual ~nGIReportGenerator() {}
protected:
	virtual double MakeHeader(Cairo::RefPtr<Cairo::Context> cr, std::string name);
	void ProcessingModules(Cairo::RefPtr<Cairo::Context> cr, nGIConfig *config);

	Gtk_addons::ContextPlotter m_OscillationPlot;
	Gtk_addons::ContextImageViewer m_ImageRenderer;
};
#endif
