//<LICENSE>

#include "../include/ngigenerator.h"

nGIGenerator::nGIGenerator()
{
}

kipl::base::TImage<float,3> nGIGenerator::GeneratePhaseSteps(kipl::base::TImage<float,2> &bias,
                                                kipl::base::TImage<float,2> &ampl,
                                                kipl::base::TImage<float,2> &phase,
                                                int steps,
                                                int periods,
                                                bool full_osc)
{
    size_t dims[3]={size_t(bias.Size(0)), size_t(bias.Size(1)),size_t(steps)};
    kipl::base::TImage<float,3> phasesteps(dims);

    return phasesteps;
}
