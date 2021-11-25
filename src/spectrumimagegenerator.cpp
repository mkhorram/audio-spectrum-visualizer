#include "spectrumimagegenerator.h"

SpectrumImageGenerator::SpectrumImageGenerator() : m_buffer(500)
{

}

void SpectrumImageGenerator::insertNewSpectrumRow(std::shared_ptr<std::vector<std::complex<double> > > FFTOutput, double ratio)
{
    //m_buffer.insert(FFTOutput);
    //
}
