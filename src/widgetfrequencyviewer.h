#ifndef WIDGETFREQUENCYVIEWER_H
#define WIDGETFREQUENCYVIEWER_H

#include <QPainter>
#include <QResizeEvent>
#include <QWidget>

#include "spectrumimagegenerator.h"
#include "common.hpp"

class WidgetFrequencyViewer : public QWidget
{
    Q_OBJECT
private:
    SpectrumImageGenerator m_imageGenerator;
    FFTAmplitudeToPixelMixingType m_mixingType;

public:
    explicit WidgetFrequencyViewer(QWidget *parent = nullptr);
    void setAmplitudeMixingType(FFTAmplitudeToPixelMixingType mixingType)
    { m_mixingType = mixingType;    m_imageGenerator.setAmplitudeMixingType(mixingType); }
    void start(int rowHeight, int firstRowHeight)
    { m_imageGenerator.startGenerator(this->width(), this->height(), rowHeight, firstRowHeight); }
    void stop()
    { m_imageGenerator.stopGenerator(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
    void insertNewSpectrumRow(FFTAnalysisResult FFTOutput);
};

#endif // WIDGETFREQUENCYVIEWER_H
