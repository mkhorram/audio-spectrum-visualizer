#ifndef WIDGETAMPLITUDE_H
#define WIDGETAMPLITUDE_H

#include <QWidget>

#include "ringbuffer.hpp"

class WidgetAmplitude : public QWidget
{
    Q_OBJECT
    struct barAmplitudeValues
    {
        float lowInput;
        float highInput;
    };

public:
    explicit WidgetAmplitude(QWidget *parent = nullptr);

    void setLevelRagne(float minValue, float maxValue);
    void setLevelBar(float lowInput, float highInput);

signals:

public slots:

private:
    float m_minValue;
    float m_maxValue;
    RingBuffer<barAmplitudeValues> amplitudes;
};

#endif // WIDGETAMPLITUDE_H
