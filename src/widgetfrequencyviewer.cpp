#include "widgetfrequencyviewer.h"

WidgetFrequencyViewer::WidgetFrequencyViewer(QWidget *parent) : QWidget(parent)
{
    m_imageGenerator.setImageSize(this->width(), this->height());
}

void WidgetFrequencyViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int imgTop, imgLeft, imgHeight, imgWidth;
    QImage img = m_imageGenerator.getImage(imgTop, imgLeft, imgHeight, imgWidth);
    painter.drawImage(QPoint(0,0), img, QRect(imgLeft, imgTop, imgWidth, imgHeight));
}

void WidgetFrequencyViewer::resizeEvent(QResizeEvent *event)
{
    m_imageGenerator.setImageSize(event->size().width(), event->size().height());
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    this->update();
}

void WidgetFrequencyViewer::insertNewSpectrumRow(FFTAnalysisResult FFTOutput)
{
    m_imageGenerator.insertNewSpectrumRow(FFTOutput);
    update();
}
