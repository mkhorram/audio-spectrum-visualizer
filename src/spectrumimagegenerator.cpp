#include "spectrumimagegenerator.h"

SpectrumImageGenerator::SpectrumImageGenerator() :
    m_buffer(5000), m_isRunning(false), m_rowsToBeDrawn(0)
{ }

void SpectrumImageGenerator::createWholeImage(int imageWidth, int imageHeight)
{
    m_wholeImageWidth = imageWidth;
    m_wholeImageHeight = (2*imageHeight)+(m_firstRowHeight+m_rowHeight);
    m_wholeImage = QImage(m_wholeImageWidth, m_wholeImageHeight, QImage::Format_RGB32);
    // TODO: remove after the jobLoop completed
    for (int x=0; x<imageWidth; x++)
        for (int y=0; y<imageHeight; y++)
            m_wholeImage.setPixelColor(x, y, QColor(x%256, y%256, (x+x*y+y)%256));
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
                // pick the row and buffer field
                {
                    // find the range of frequencies for every pixel
                    // detect the highest value in the range
                    // set the pixel color from formulae
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void SpectrumImageGenerator::insertNewSpectrumRow(FFTAnalysisResult FFTOutput)
{
    std::lock_guard<std::mutex> guardBuffer(m_mutexBuffer);
    m_buffer.insert(FFTOutput);
}

void SpectrumImageGenerator::runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight)
{
    m_rowHeight = rowHeight;
    m_firstRowHeight = firstRowHeight;

    setImageSize(imageWidth, imageHeight);

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

void SpectrumImageGenerator::setImageSize(int imageWidth, int imageHeight)
{
    std::lock_guard<std::mutex> guardImage(m_mutexImage);
    createWholeImage(imageWidth, imageHeight);
    m_subAreaTop = 0;
    m_subAreaHeight = imageHeight;
    // redraw the subArea
}

QImage &SpectrumImageGenerator::getImage(int &top, int &left, int &height, int &width)
{
    std::lock_guard<std::mutex> guardImage(m_mutexImage);
    top = m_subAreaTop;
    left = m_subAreaLeft;
    height = m_subAreaHeight;
    width = m_subAreaWidth;
    return m_wholeImage;
}
