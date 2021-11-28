#include "spectrumimagegenerator.h"

SpectrumImageGenerator::SpectrumImageGenerator() : m_buffer(5000)
{ }

void SpectrumImageGenerator::createImage(int imageWidth, int imageHeight)
{
    m_spectrumImage = QImage(imageWidth, imageHeight, QImage::Format_RGB32);
    for (int x=0; x<imageWidth; x++)
        for (int y=0; y<imageHeight; y++)
            m_spectrumImage.setPixelColor(x, y, QColor(x%256, y%256, (x+x*y+y)%256));
}

void SpectrumImageGenerator::insertNewSpectrumRow(FFTAnalysisResult FFTOutput)
{
    m_buffer.insert(FFTOutput);
    // update image
}

void SpectrumImageGenerator::runGenerator(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight)
{
    createImage(imageWidth, imageHeight);
    // start

    m_imageTop = 0;
    m_imageHeight = imageHeight;
    m_rowHeight = rowHeight;
    m_firstRowHeight = firstRowHeight;
    m_imageWriteTop = m_imageHeight;
}

void SpectrumImageGenerator::stopGenerator()
{
    // stop
}

void SpectrumImageGenerator::setImageSize(int imageWidth, int imageHeight, int rowHeight, int firstRowHeight)
{
    createImage(imageWidth, imageHeight);
}

QImage SpectrumImageGenerator::getImage(int &top, int &height)
{
    top = m_imageTop;
    height = m_imageHeight;
    return m_spectrumImage;
}
