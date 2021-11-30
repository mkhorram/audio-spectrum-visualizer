#include "spectrumimagegenerator.h"

SpectrumImageGenerator::SpectrumImageGenerator() :
    m_buffer(5000), m_isRunning(false), m_rowsToBeDrawn(0)
{ }

void SpectrumImageGenerator::createImage(int imageWidth, int imageHeight)
{
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_spectrumImage = QImage(imageWidth,
                             (2*imageHeight)+(m_firstRowHeight+m_rowHeight),
                             QImage::Format_RGB32);
    for (int x=0; x<imageWidth; x++)
        for (int y=0; y<imageHeight; y++)
            m_spectrumImage.setPixelColor(x, y, QColor(x%256, y%256, (x+x*y+y)%256));
}

void SpectrumImageGenerator::jobLoop()
{
    while (m_isRunning)
    {
        if (m_rowsToBeDrawn > 0)
        {
            std::lock_guard<std::mutex> guardBuffer(m_mutexBuffer);
            std::lock_guard<std::mutex> guardImage(m_mutexImage);
            for ( ; m_rowsToBeDrawn > 0; m_rowsToBeDrawn--)
            {
                // draw new rows
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void SpectrumImageGenerator::insertNewSpectrumRow(FFTAnalysisResult FFTOutput)
{
    std::lock_guard<std::mutex> guardBuffer(m_mutexBuffer);
    m_buffer.insert(FFTOutput);
    // update image
}

void SpectrumImageGenerator::runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight)
{
    m_imageTop = 0;
    m_imageHeight = imageHeight;
    m_rowHeight = rowHeight;
    m_firstRowHeight = firstRowHeight;
    m_imageWriteTop = m_imageHeight;

    createImage(imageWidth, imageHeight);

    m_isRunning = true;
    std::thread thr(jobLoopCaller, this);
    m_loopThread = std::move(thr);
}

void SpectrumImageGenerator::stopGenerator()
{
    m_isRunning = false;
    if (m_loopThread.joinable())
        m_loopThread.join();
}

void SpectrumImageGenerator::setImageSize(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight)
{
    createImage(imageWidth, imageHeight);
}

QImage &SpectrumImageGenerator::getImage(int &top, int &height)
{
    std::lock_guard<std::mutex> guardImage(m_mutexImage);
    top = m_imageTop;
    height = m_imageHeight;
    return m_spectrumImage;
}
