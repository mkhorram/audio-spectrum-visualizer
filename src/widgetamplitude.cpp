#include "widgetamplitude.h"

WidgetAmplitude::WidgetAmplitude(QWidget *parent) : QWidget(parent)
{

}

void WidgetAmplitude::setLevelRagne(float minValue, float maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void WidgetAmplitude::setLevelBar(float lowInput, float highInput)
{

}
