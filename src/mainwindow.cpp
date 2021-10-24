#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toolBar->addWidget(ui->tbSettings);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(ui->tbStart);
    ui->toolBar->addWidget(ui->tbStop);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(ui->lblStopRunStatus);
    ui->toolBar->addWidget(ui->lblSampleRate);

    ui->statusbar->addWidget(ui->lblConfiguredAudioFormat);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    m_wgtAmplitude = new WidgetAmplitude(this);
    m_wgtFrequencyViewer = new WidgetFrequencyViewer(this);
    splitter->addWidget(m_wgtAmplitude);
    splitter->addWidget(m_wgtFrequencyViewer);
    splitter->setStretchFactor(0,4);
    splitter->setStretchFactor(1,1);
    this->setCentralWidget( splitter );

    connect(&m_dlgSettings, &DialogSettings::DialogSettingsClosed, this, &MainWindow::dialogueSettingsClosed);

    ChangeWorkingState(WorkState::Stopped);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_dlgSettings.isVisible())
        m_dlgSettings.close();
    event->accept();
}

void MainWindow::on_tbSettings_clicked()
{
    ChangeWorkingState(WorkState::ShowingSettingsDialogue);
    (m_dlgSettings.isVisible())?  m_dlgSettings.hide() : m_dlgSettings.show();
}

void MainWindow::dialogueSettingsClosed(QCloseEvent *event)
{
    Q_UNUSED(event)
    ChangeWorkingState(WorkState::Stopped);
}

void MainWindow::ChangeWorkingState(WorkState workingState)
{
    if (workingState == WorkState::ShowingSettingsDialogue ||
            workingState == WorkState::NoSettings)
    {
        ui->tbStart->setEnabled(false);
        ui->tbStop->setEnabled(false);
        ui->tbSettings->setEnabled(true);
        m_workingState = workingState;
    }
    else if (workingState == WorkState::Stopped)
    {
        ui->tbStart->setEnabled(true);
        ui->tbStop->setEnabled(false);
        ui->tbSettings->setEnabled(true);
        m_workingState = workingState;
    }
    else
    {
        ui->tbStart->setEnabled(false);
        ui->tbStop->setEnabled(true);
        ui->tbSettings->setEnabled(false);
        m_workingState = workingState;
    }
}

void MainWindow::on_tbStart_clicked()
{
    QAudioFormat format = m_dlgSettings.getFormat();
    QAudioDeviceInfo m_deviceInfo = m_dlgSettings.getDeviceInfo();
    m_audioHandler.start(format, m_deviceInfo);
}
