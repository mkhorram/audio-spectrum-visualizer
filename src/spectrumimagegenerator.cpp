#include "spectrumimagegenerator.h"

#include <QDebug>

SpectrumImageGenerator::SpectrumImageGenerator() :
    m_buffer(5000), m_rowsToBeDrawn(0)
{ }

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

void SpectrumImageGenerator::findMeanAndMaxAbsoluteValue(FFTAnalysisResult &bufferRow, double &maxVal, double &averageVal)
{
    maxVal = 0;
    double sumVal = 0;
    for (unsigned long long i = 0; i < bufferRow.rowBuffer->size(); ++i) {
        double val = std::abs(bufferRow.rowBuffer->at(i));
        sumVal += val;
        if (maxVal < val)
            maxVal = val;
    }
    averageVal = sumVal / bufferRow.rowBuffer->size();
}

std::vector<QColor> SpectrumImageGenerator::generateRowColors(FFTAnalysisResult &bufferRow, double &maxVal, double &averageVal)
{
    std::vector<QColor> colors;
    for (FFTRangeToPixelMap rangeToPixel : m_FFTtoPixelConversionRanges)
    {
        int n2 = std::min((rangeToPixel.subFFTRangeStart + 1) + rangeToPixel.subFFTRangeLength,
                          long(bufferRow.rowBuffer->size()));
        double targetValue = 0;
        std::vector<std::complex<double>> &rowBuffer = *bufferRow.rowBuffer;
        if (m_mixingType == FFTAmplitudeToPixelMixingType::Average)
        {
            for (int i=rangeToPixel.subFFTRangeStart; i<n2; ++i)
                targetValue += std::abs(rowBuffer[i]);
            if (targetValue > 0)
                targetValue /= (n2 - rangeToPixel.subFFTRangeStart);
        }
        else if (m_mixingType == FFTAmplitudeToPixelMixingType::MaximumValue)
        {
            for (int i=rangeToPixel.subFFTRangeStart; i<n2; ++i)
                targetValue = std::max(std::abs(rowBuffer[i]), targetValue);
        }
        else if (m_mixingType == FFTAmplitudeToPixelMixingType::SquareAverage)
        {
            for (int i=rangeToPixel.subFFTRangeStart; i<n2; ++i)
                targetValue += std::abs(rowBuffer[i]);
            if (targetValue > 0)
                targetValue = std::sqrt(targetValue / (n2 - rangeToPixel.subFFTRangeStart));
        }
        // TODO: find appropiate formulae
        int r = targetValue;
        int g = targetValue;
        int b = targetValue;
        QColor pixColor(r,g,b);
        for (int j=0; j<rangeToPixel.pixelCount; ++j)
            colors.push_back(pixColor);
    }
    return colors;
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
            int workCounter = std::min(readableBuffer, m_rowsToBeDrawn);
            for ( int i = workCounter; i > 0; --i)
            {
                // pick the row
                FFTAnalysisResult bufferRow = m_buffer[readableBuffer - i];
                {
                    // find the maximum and average in bufferRow
                    double maxVal, averageVal;
                    findMeanAndMaxAbsoluteValue(bufferRow, maxVal, averageVal);
                    // set the pixel color from formulae
                    std::vector<QColor> colors = generateRowColors(bufferRow, maxVal, averageVal);
                    // TODO: draw the pixels
                }
            }
            m_rowsToBeDrawn = 0;
            m_imageUpdated = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void SpectrumImageGenerator::insertNewSpectrumRow(FFTAnalysisResult FFTOutput)
{
    std::lock_guard<std::mutex> guardBuffer(m_mutexBuffer);
    m_buffer.insert(FFTOutput);
    m_rowsToBeDrawn++;
}

void SpectrumImageGenerator::startGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight)
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
