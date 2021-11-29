#ifndef SPECTRUMIMAGEGENERATOR_H
#define SPECTRUMIMAGEGENERATOR_H

#include <QImage>

#include <complex>
#include <memory>
#include <thread>
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
    int m_rowHeight;
    int m_firstRowHeight;
    int m_imageWriteTop;

    bool m_isRunning;
    std::thread m_loopThread;

private:
    void createImage(int imageWidth, int imageHeight);
    static void jobLoop(SpectrumImageGenerator *imgGenerator)
    {
        while (imgGenerator->m_isRunning)
        {
            //
        }
    }

public:
    SpectrumImageGenerator();
    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
    void runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    void stopGenerator();
    void setImageSize(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    QImage &getImage(int &top, int &height);
};

#endif // SPECTRUMIMAGEGENERATOR_H
