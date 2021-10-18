#ifndef WIDGETAMPLITUDE_H
#define WIDGETAMPLITUDE_H

#include <QWidget>

#include "ringbuffer.hpp"

class WidgetAmplitude : public QWidget
{
    Q_OBJECT
    struct BarAmplitudeValues
    {
        BarAmplitudeValues(float low_Input, float high_Input)
        {
            lowInput = low_Input;
            highInput = high_Input;
        }
        float lowInput;
        float highInput;
    };

public:
    explicit WidgetAmplitude(QWidget *parent = nullptr);

    void setLevelRagne(float minValue, float maxValue);
    void insertLevelBar(float lowInput, float highInput);

signals:

public slots:

private:
    float m_minValue;
    float m_maxValue;
    RingBuffer<BarAmplitudeValues> m_amplitudes;
};

#endif // WIDGETAMPLITUDE_H
