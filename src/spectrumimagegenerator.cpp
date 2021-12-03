#include "spectrumimagegenerator.h"

SpectrumImageGenerator::SpectrumImageGenerator() :
    m_buffer(5000), m_rowsToBeDrawn(0)
{ }
#include <QPainter>
#include <QPixmap>
void SpectrumImageGenerator::createWholeImage(int imageWidth, int imageHeight)
{
    m_wholeImageWidth = imageWidth;
    m_wholeImageHeight = (3*imageHeight)+(m_firstRowHeight+m_rowHeight);
    m_wholeImage = QImage(m_wholeImageWidth, m_wholeImageHeight, QImage::Format_ARGB32);

    // TODO: remove after the jobLoop completed
    for (int x=0; x<imageWidth; x++)
        for (int y=0; y<imageHeight; y++)
            m_wholeImage.setPixelColor(x, y, QColor(((x*y+100)/15)%256, ((x*y)/15)%256, ((m_wholeImageWidth*x*y)/(x+50))%256));
}

void SpectrumImageGenerator::jobLoop()
{
    while (m_isRunning)
    {
        if (m_rowsToBeDrawn > 0)
        {
            std::lock_guard<std::mutex> guardBuffer(m_mutexBuffer);
            std::lock_guard<std::mutex> guardImage(m_mutexImage);
            int readableBuffer = m_buffer.getlenghtToRead();
            if (readableBuffer)
            for ( ; m_rowsToBeDrawn > 0; m_rowsToBeDrawn--)
            {
                // pick the row and buffer field
                {
                    // find the range of frequencies for every pixel
                    // detect the highest value in the range
                    // set the pixel color from formulae
                }
            }
            m_imageUpdated = true;
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
    m_imageUpdated = false;
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
    m_subAreaLeft = 0;
    m_subAreaTop = 0;
    m_subAreaWidth = imageWidth;
    m_subAreaHeight = imageHeight;
    // TODO: redraw the subArea
    m_imageUpdated = false;
}

// TODO: Should be replaced with direct draw on the QWidget?
QImage &SpectrumImageGenerator::getImage(int &imgLeft, int &imgTop, int &imgWidth, int &imgHeight)
{
    std::lock_guard<std::mutex> guardImage(m_mutexImage);
    imgLeft = m_subAreaLeft;
    imgTop = m_subAreaTop;
    imgWidth = m_subAreaWidth;
    imgHeight = m_subAreaHeight;
    m_imageUpdated = false;
    return m_wholeImage;
}
