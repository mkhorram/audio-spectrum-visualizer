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
    QImage m_spectrumImage;
    int m_imageTop;
    int m_imageHeight;
    int m_imageWriteTop;

private:

public:
    SpectrumImageGenerator();
    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
    void runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    void stopGenerator();
    void setImageSize(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    QImage getImage();
};

#endif // SPECTRUMIMAGEGENERATOR_H
