#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include "dialogsettings.h"
#include "widgetamplitude.h"
#include "widgetfrequencyviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tbSettings_clicked();

private:
    Ui::MainWindow *ui;
    WidgetAmplitude *m_wgtAmplitude;
    WidgetFrequencyViewer *m_wgtFrequencyViewer;
    DialogSettings m_dlgSettings;
};

#endif // MAINWINDOW_H
