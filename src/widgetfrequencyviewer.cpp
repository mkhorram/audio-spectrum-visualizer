#include "widgetfrequencyviewer.h"

WidgetFrequencyViewer::WidgetFrequencyViewer(QWidget *parent) : QWidget(parent)
{
    m_imageBuffer = QImage(this->width(), this->height(), QImage::Format_RGB32);
}

void WidgetFrequencyViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), m_imageBuffer);
}

void WidgetFrequencyViewer::resizeEvent(QResizeEvent *event)
{
    m_imageBuffer = QImage(event->size().width(), event->size().height(), QImage::Format_RGB32);
    QColor color((event->size().width()*5)%256, event->size().height()%256, (event->size().width()+event->size().height())%255);
    m_imageBuffer.fill(color);
}

void WidgetFrequencyViewer::insertNewSpectrumRow(FFTAnalysisResult FFTOutput)
{
    m_imageGenerator.insertNewSpectrumRow(FFTOutput);
    update();
}
