#ifndef SPECTRUMIMAGEGENERATOR_H
#define SPECTRUMIMAGEGENERATOR_H

#include <QImage>

#include <chrono>
#include <cmath>
#include <complex>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "common.hpp"
#include "ringbuffer.hpp"


class SpectrumImageGenerator
{

private:
    RingBuffer<FFTAnalysisResult> m_buffer;
    FFTAmplitudeToPixelMixingType m_mixingType;
    std::vector<FFTRangeToPixelMap> m_FFTtoPixelConversionRanges;

    bool m_imageUpdated = false;

    int m_rowHeight = 2;
    int m_firstRowHeight = 5;

    QImage m_wholeImage;
    int m_wholeImageWidth = 0;
    int m_wholeImageHeight = 0;

    int m_subAreaWidth = 0;
    int m_subAreaHeight = 0;
    int m_subAreaTop = 0;
    int m_subAreaLeft = 0;

    int m_reserveSubAreaTop = 0;

    bool m_isRunning = false;
    int m_rowsToBeDrawn;
    std::mutex m_mutexBuffer;
    std::mutex m_mutexImage;
    std::thread m_loopThread;

public:
    SpectrumImageGenerator();

    void setAmplitudeMixingType(FFTAmplitudeToPixelMixingType mixingType)
    { m_mixingType = mixingType; }
    void setFFTRanges(std::vector<FFTRangeToPixelMap> &FFTtoPixelConversionRanges)
    {
        std::lock_guard<std::mutex> guardImage(m_mutexImage);
        m_FFTtoPixelConversionRanges = FFTtoPixelConversionRanges;
    }

    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
    void startGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    void stopGenerator();
    void setImageSize(int imageWidth, int imageHeight);
    QImage &getImage(int &imgLeft, int &imgTop, int &imgWidth, int &imgHeight);

private:
    void createWholeImage(int imageWidth, int imageHeight);
    inline std::vector<QColor> generateRowColors(FFTAnalysisResult &bufferRow);
    void jobLoop();

    static void jobLoopCaller(SpectrumImageGenerator *imgGenerator)
    {
        imgGenerator->jobLoop();
    }
};

#endif // SPECTRUMIMAGEGENERATOR_H
