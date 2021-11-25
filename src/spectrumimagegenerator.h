#ifndef SPECTRUMIMAGEGENERATOR_H
#define SPECTRUMIMAGEGENERATOR_H

#include <QImage>

#include <complex>
#include <memory>
#include <vector>

#include "ringbuffer.hpp"

class SpectrumImageGenerator
{
    struct FFTAnalysis{
        std::shared_ptr<std::vector<std::complex<double>>> rowBuffer;
        double ratio;
    };

private:
    RingBuffer<FFTAnalysis> m_buffer;
public:
    SpectrumImageGenerator();
    void insertNewSpectrumRow(std::shared_ptr<std::vector<std::complex<double>>> FFTOutput, double ratio);
    void runGenerator(unsigned int imageWidth, unsigned int imageRows, unsigned int rowHeight, unsigned int firstRowHeight);
    void stopGenerator();
    void resizeImage(unsigned int imageWidth, unsigned int imageRows, unsigned int rowHeight, unsigned int firstRowHeight);
};

#endif // SPECTRUMIMAGEGENERATOR_H
