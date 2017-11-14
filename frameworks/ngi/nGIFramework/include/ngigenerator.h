#ifndef NGIGENERATOR_H
#define NGIGENERATOR_H

#include <base/timage.h>

class nGIGenerator
{
public:
    nGIGenerator();

    kipl::base::TImage<float,3> GeneratePhaseSteps(kipl::base::TImage<float,2> &bias,
                                                    kipl::base::TImage<float,2> &ampl,
                                                    kipl::base::TImage<float,2> &phase,
                                                    int steps=9,
                                                    int periods=1,
                                                    bool full_osc=true);
};

#endif // NGIGENERATOR_H
