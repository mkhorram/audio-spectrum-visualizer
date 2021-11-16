#ifndef WIDGETAMPLITUDE_H
#define WIDGETAMPLITUDE_H

#include <QWidget>
#include <QPainter>

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
    RingBuffer<BarAmplitudeValues> m_amplitudes;

    int m_leftMargin = 10;
    int m_rightMargin = 10;
    int m_topMargin = 10;
    int m_bottomMargin = 10;

    int rowHight = 2;
    int newRowHight = 5;

public:
    explicit WidgetAmplitude(QWidget *parent = nullptr, unsigned long bufSize = 5000);

    void setLevelRagne(double rangeMinValue, double rangeMaxValue);
    void insertLevelBar(double lowInput, double highInput);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:

};

#endif // WIDGETAMPLITUDE_H
