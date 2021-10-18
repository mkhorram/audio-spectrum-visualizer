#include "widgetamplitude.h"

WidgetAmplitude::WidgetAmplitude(QWidget *parent) : QWidget(parent), m_amplitudes(5000)
{

}

void WidgetAmplitude::setLevelRagne(float minValue, float maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void WidgetAmplitude::insertLevelBar(float lowInput, float highInput)
{
    BarAmplitudeValues bav(lowInput, highInput);
    m_amplitudes.insert(bav);
}
