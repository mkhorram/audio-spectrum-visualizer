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
    QImage m_spectrumImage;
    int m_imageWidth;
    int m_imageHeight;
    int m_imageTop;
    int m_rowHeight;
    int m_firstRowHeight;
    int m_imageWriteTop;

    bool m_isRunning;
    int m_rowsToBeDrawn;
    std::mutex m_mutexBuffer;
    std::mutex m_mutexImage;
    std::thread m_loopThread;

public:
    SpectrumImageGenerator();
    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
    void runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    void stopGenerator();
    void setImageSize(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight);
    QImage &getImage(int &top, int &height);

private:
    void createImage(int imageWidth, int imageHeight);
    void jobLoop();

    static void jobLoopCaller(SpectrumImageGenerator *imgGenerator)
    {
        imgGenerator->jobLoop();
    }
};

#endif // SPECTRUMIMAGEGENERATOR_H
