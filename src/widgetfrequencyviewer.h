#ifndef WIDGETFREQUENCYVIEWER_H
#define WIDGETFREQUENCYVIEWER_H

#include <QPainter>
#include <QResizeEvent>
#include <QWidget>

#include "spectrumimagegenerator.h"

class WidgetFrequencyViewer : public QWidget
{
    Q_OBJECT
private:
    QImage m_imageBuffer;
    SpectrumImageGenerator m_imageGenerator;

public:
    explicit WidgetFrequencyViewer(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
    void insertNewSpectrumRow(std::shared_ptr<std::vector<std::complex<double>>> FFTOutput, double ratio);
};

#endif // WIDGETFREQUENCYVIEWER_H
