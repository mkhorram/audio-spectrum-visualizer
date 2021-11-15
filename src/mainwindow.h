#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QObject>
#include <QSplitter>

#include <QPushButton>

#include "audioinputhandler.h"
#include "dialogsettings.h"
#include "widgetamplitude.h"
#include "widgetfrequencyviewer.h"

enum class WorkState
{
    Running,
    Stopped,
    NoSettings,
    ShowingSettingsDialogue
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void closeEvent(QCloseEvent *event);
    void on_tbSettings_clicked();
    void dialogueSettingsClosed(QCloseEvent *event);

private slots:
    void on_tbStart_clicked();
    void on_tbStop_clicked();

    void actualSampleRateEstimated(long actualSampleRate);

private:
    Ui::MainWindow *ui;
    WidgetAmplitude *m_wgtAmplitude;
    WidgetFrequencyViewer *m_wgtFrequencyViewer;
    DialogSettings m_dlgSettings;

    AudioInputHandler m_audioHandler;
    WorkState m_workingState;

private:
    void ChangeWorkingState(WorkState workingState);
};

#endif // MAINWINDOW_H
