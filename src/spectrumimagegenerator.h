#ifndef SPECTRUMIMAGEGENERATOR_H
#define SPECTRUMIMAGEGENERATOR_H

#include <QImage>

#include <complex>
#include <memory>
#include <vector>

#include "ringbuffer.hpp"
#include "common.hpp"


class SpectrumImageGenerator
{

private:
    RingBuffer<FFTAnalysisResult> m_buffer;
public:
    SpectrumImageGenerator();
    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
    void runGenerator(unsigned int imageWidth, unsigned int imageRows, unsigned int rowHeight, unsigned int firstRowHeight);
    void stopGenerator();
    void resizeImage(unsigned int imageWidth, unsigned int imageRows, unsigned int rowHeight, unsigned int firstRowHeight);
};

#endif // SPECTRUMIMAGEGENERATOR_H
