#ifndef SPECTRUMIMAGEGENERATOR_H
#define SPECTRUMIMAGEGENERATOR_H

#include <QImage>

#include <complex>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "ringbuffer.hpp"
#include "common.hpp"


class SpectrumImageGenerator
{

private:
    RingBuffer<FFTAnalysisResult> m_buffer;
    FFTAmplitudeToPixelMixingType m_mixingType;
    bool m_imageUpdated;

    int m_rowHeight;
    int m_firstRowHeight;

    QImage m_wholeImage;
    int m_wholeImageWidth;
    int m_wholeImageHeight;

    int m_subAreaWidth;
    int m_subAreaHeight;
    int m_subAreaTop;
    int m_subAreaLeft;

    int m_reserveSubAreaTop;

    bool m_isRunning;
    int m_rowsToBeDrawn;
    std::mutex m_mutexBuffer;
    std::mutex m_mutexImage;
    std::thread m_loopThread;

public:
    SpectrumImageGenerator();

    void setAmplitudeMixingType(FFTAmplitudeToPixelMixingType mixingType)
    { m_mixingType = mixingType; }

    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
    void runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    void stopGenerator();
    void setImageSize(int imageWidth, int imageHeight);
    QImage &getImage(int &imgTop, int &imgLeft, int &imgHeight, int &imgWidth);

private:
    void createWholeImage(int imageWidth, int imageHeight);
    void jobLoop();

    static void jobLoopCaller(SpectrumImageGenerator *imgGenerator)
    {
        imgGenerator->jobLoop();
    }
};

#endif // SPECTRUMIMAGEGENERATOR_H
