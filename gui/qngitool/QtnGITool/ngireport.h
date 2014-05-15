#ifndef NGIREPORT_H
#define NGIREPORT_H

#include <reportgeneratorbase.h>
#include <nGIConfig.h>
#include <base/timage.h>
#include <string>
#include <QString>
#include <imagepainter.h>
#include <plotpainter.h>

void test_nGIReport();

class nGIReport : public QtAddons::ReportGeneratorBase
{
public:
    nGIReport();
    int CreateReport(QString filename, std::string projname, nGIConfig *config,
                        kipl::base::TImage<float,2> ampl,
                        kipl::base::TImage<float,2> dpc,
                        kipl::base::TImage<float,2> dfi,
                        float visibility,
                        float *axis,
                        float *ref_osc,
                        float *sample_osc);

protected:
    void ProcessingModules(nGIConfig *config);
    QtAddons::PlotPainter m_OscillationPlot;
};

#endif // NGIREPORT_H
