#ifndef WIDGETAMPLITUDE_H
#define WIDGETAMPLITUDE_H

#include <QWidget>
#include <QPainter>

#include <cmath>

#include "ringbuffer.hpp"

class WidgetAmplitude : public QWidget
{
    Q_OBJECT
    struct BarAmplitudeValues
    {
        BarAmplitudeValues(double low_Input = 0, double high_Input = 0)
        {
            lowInput = low_Input;
            highInput = high_Input;
        }
        double lowInput;
        double highInput;
    };

private:
    double m_rangeMinValue;
    double m_rangeMaxValue;
    RingBuffer<BarAmplitudeValues> m_amplitudesBuffer;

    int m_leftMargin = 5;
    int m_rightMargin = 5;
    int m_topMargin = 5;
    int m_bottomMargin = 5;

    unsigned int m_barThickness = 2;
    unsigned int m_newBarThickness = 5;

public:
    explicit WidgetAmplitude(QWidget *parent = nullptr, unsigned long bufSize = 500);

    void setLevelRagne(double rangeMinValue, double rangeMaxValue);
    void setBarThicknesses(unsigned int barThickness, unsigned int newBarThickness)
    {m_barThickness = barThickness;    m_newBarThickness = newBarThickness;}

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
    void insertLevelBar(double lowInput, double highInput);

};

#endif // WIDGETAMPLITUDE_H
