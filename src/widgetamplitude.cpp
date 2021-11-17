#include "widgetamplitude.h"

WidgetAmplitude::WidgetAmplitude(QWidget *parent, unsigned long bufSize) : QWidget(parent), m_amplitudesBuffer(bufSize)
{

}

void WidgetAmplitude::setLevelRagne(double rangeMinValue, double rangeMaxValue)
{
    m_rangeMinValue = rangeMinValue;
    m_rangeMaxValue = rangeMaxValue;
}

void WidgetAmplitude::insertLevelBar(double lowInput, double highInput)
{
    BarAmplitudeValues bav(lowInput, highInput);
    m_amplitudesBuffer.insert(bav);
}

void WidgetAmplitude::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int drawLeftEdge = painter.viewport().left() + m_leftMargin;
    int drawTopEdge = painter.viewport().top() + m_topMargin;
    int drawWidth = painter.viewport().right() - (m_rightMargin+m_leftMargin);
    int drawHeight = painter.viewport().bottom() - (m_bottomMargin+m_topMargin);

    unsigned long readableLength = m_amplitudesBuffer.getlenghtToRead();
    int barTop = drawTopEdge+drawHeight-m_newBarThickness;
    for (int i = readableLength-2; i>=0; --i)
    {}

    painter.setPen(Qt::black);
    painter.drawRect(drawLeftEdge, drawTopEdge, drawWidth, drawHeight);
}
