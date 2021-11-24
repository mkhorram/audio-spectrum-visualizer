#include "widgetamplitude.h"


WidgetAmplitude::WidgetAmplitude(QWidget *parent, unsigned long bufSize) : QWidget(parent), m_amplitudesBuffer(bufSize)
{

}

void WidgetAmplitude::setLevelRagne(double rangeMinValue, double rangeMaxValue)
{
    m_rangeMinValue = rangeMinValue;
    m_rangeMaxValue = rangeMaxValue;
    m_amplitudesBuffer.reset();
}

void WidgetAmplitude::insertLevelBar(double lowInput, double highInput)
{
    BarAmplitudeValues bav(lowInput, highInput);
    m_amplitudesBuffer.insert(bav);
    this->update();
}

void WidgetAmplitude::paintEvent(QPaintEvent *event)
{
    unsigned long readableLength = m_amplitudesBuffer.getlenghtToRead();
    if (readableLength == 0)
        return;

    QPainter painter(this);

    int drawLeftEdge = painter.viewport().left() + m_leftMargin;
    int drawTopEdge = painter.viewport().top() + m_topMargin;
    int drawWidth = painter.viewport().right() - (m_rightMargin+m_leftMargin);
    int drawHeight = painter.viewport().bottom() - (m_bottomMargin+m_topMargin);

    painter.fillRect(painter.viewport().left(),
                     painter.viewport().top(),
                     painter.viewport().right(),
                     painter.viewport().bottom(), QColor(0,10,80));

    unsigned long newestIndex = readableLength - 1;
    int barTop = drawTopEdge + drawHeight - m_newBarThickness;
    int barLeft = drawLeftEdge + (m_amplitudesBuffer[newestIndex].lowInput - m_rangeMinValue) / (m_rangeMaxValue-m_rangeMinValue) * drawWidth;
    int barWidth = (m_amplitudesBuffer[newestIndex].highInput - m_amplitudesBuffer[newestIndex].lowInput) / (m_rangeMaxValue-m_rangeMinValue) * drawWidth;
    painter.fillRect(barLeft, barTop, barWidth, m_newBarThickness, QColor(255,190,190));

    unsigned long barIndex = newestIndex;
    while (barIndex > 0)
    {
        double &lowInput = m_amplitudesBuffer[barIndex].lowInput;
        double &highInput = m_amplitudesBuffer[barIndex].highInput;
        barTop -= m_barThickness;
        if (barTop < drawTopEdge)
            break;
        barLeft = drawLeftEdge + (lowInput - m_rangeMinValue) / (m_rangeMaxValue-m_rangeMinValue) * drawWidth;
        int barWidth = (highInput - lowInput) / (m_rangeMaxValue-m_rangeMinValue) * drawWidth;
        painter.fillRect(barLeft, barTop, barWidth, m_barThickness, QColor(110,110,255));
        --barIndex;
    }

    painter.setPen(Qt::black);
    painter.drawRect(drawLeftEdge, drawTopEdge, drawWidth, drawHeight);
}
