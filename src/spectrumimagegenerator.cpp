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
            m_wholeImage.setPixelColor(x, y, QColor(((x*y+100)/25)%256, ((x*y)/25)%256, ((m_wholeImageWidth*x*y+x)/(x+50))%256));
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
    std::vector<std::complex<double>> &rowBuffer = *bufferRow.rowBuffer;
    double variationRange = (averageVal > 0)? 2*averageVal : 1;  // to avoid division by zero
    if (variationRange < maxVal)
        variationRange = (variationRange + maxVal) / 2;
    std::vector<QColor> colors(m_wholeImageWidth);
    for (FFTRangeToPixelMap rangeToPixel : m_FFTtoPixelConversionRanges)
    {
        int FFTSearchEnd = std::min(rangeToPixel.subFFTRangeStart + rangeToPixel.subFFTRangeCount + 1,
                          long(bufferRow.rowBuffer->size()));
        double targetValue = 0;
        if (m_mixingType == FFTAmplitudeToPixelMixingType::Average)
        {
            for (int i=rangeToPixel.subFFTRangeStart; i<FFTSearchEnd; ++i)
                targetValue += std::abs(rowBuffer[i]);
            if (FFTSearchEnd > rangeToPixel.subFFTRangeStart)
                targetValue /= (FFTSearchEnd - rangeToPixel.subFFTRangeStart);
        }
        else if (m_mixingType == FFTAmplitudeToPixelMixingType::MaximumValue)
        {
            for (int i=rangeToPixel.subFFTRangeStart; i<FFTSearchEnd; ++i)
                targetValue = std::max(std::abs(rowBuffer[i]), targetValue);
        }
        else if (m_mixingType == FFTAmplitudeToPixelMixingType::SquareAverage)
        {
            for (int i=rangeToPixel.subFFTRangeStart; i<FFTSearchEnd; ++i){
                double val = std::abs(rowBuffer[i]);
                targetValue += val*val;
            }
            if (FFTSearchEnd > rangeToPixel.subFFTRangeStart)
                targetValue = std::sqrt(targetValue / (FFTSearchEnd - rangeToPixel.subFFTRangeStart));
        }
        // TODO: find appropiate formulae
        QColor pixColor;
        double val = targetValue / variationRange;
        if (val < 1)
        {
            double &x = val;
            double x2 = x*x;
            double x3 = x2*x;
            double x4 = x2*x2;
            double x5 = x3*x2;
            int r = int(1362.17948717951 *x5 - 3594.84265734272 *x4 + 2757.502913752978*x3 - 367.3149766900062*x2 + 96.96824009324689*x + 0.2884615384605245);
            int g = int(961.5384615384775*x5 - 480.7692307692707*x4 - 1775.932400932365*x3 + 712.8496503496364*x2 + 591.6142191142214*x + 1.223776223776065);
            int b = int(2724.35897435885 *x5 - 10482.22610722582*x4 + 15206.14801864779*x3 - 9848.557692307628*x2 + 2341.081002331004*x + 70.08741258740997);
            pixColor = QColor(r,g,b);
        }
        else
            pixColor = QColor(255,255,255);

        int pixelSearchEnd = std::min(rangeToPixel.pixelStart + rangeToPixel.pixelCount + 1, m_wholeImageWidth);
        for (int i=rangeToPixel.pixelStart; i<pixelSearchEnd; ++i)
            if (i < m_wholeImageWidth)
                colors.at(i) = pixColor;
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
