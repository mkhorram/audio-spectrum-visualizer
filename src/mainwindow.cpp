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
    connect(&m_audioHandler, &AudioInputHandler::actualSampleRateEstimated, this, &MainWindow::actualSampleRateEstimated);
    connect(&m_audioHandler, &AudioInputHandler::lowHighSampleValuesComputed, m_wgtAmplitude, &WidgetAmplitude::insertLevelBar);

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
    if (workingState == WorkState::Stopped)
    {
        ui->tbStart->setEnabled(true);
        ui->tbStop->setEnabled(false);
        ui->tbSettings->setEnabled(true);
        m_workingState = workingState;
        ui->lblSampleRate->setText("Stopped");
    }
    else if (workingState == WorkState::Running)
    {
        ui->tbStart->setEnabled(false);
        ui->tbStop->setEnabled(true);
        ui->tbSettings->setEnabled(false);
        m_workingState = workingState;
        ui->lblSampleRate->setText("Running");
    }
    else //if (workingState == WorkState::ShowingSettingsDialogue || workingState == WorkState::NoSettings)
    {
        ui->tbStart->setEnabled(false);
        ui->tbStop->setEnabled(false);
        ui->tbSettings->setEnabled(true);
        m_workingState = workingState;
    }
}

void MainWindow::on_tbStart_clicked()
{
    QAudioFormat format = m_dlgSettings.getFormat();
    bool result = m_audioHandler.start(format, m_dlgSettings.getDeviceInfo(),
                                       m_dlgSettings.getFFTNeededSamples(), m_dlgSettings.getFrequencyMeasuringNeededSamples() );
    if (result)
    {
        ChangeWorkingState(WorkState::Running);

        int sampleSize = format.sampleSize();
        auto sampleType = format.sampleType();
        if (sampleSize == 8)
        {
            if (sampleType == QAudioFormat::UnSignedInt)
                m_wgtAmplitude->setLevelRagne(0, 255);
            else if (sampleType == QAudioFormat::SignedInt)
                m_wgtAmplitude->setLevelRagne(-128, 127);
        }
        else if (sampleSize == 16)
        {
            if (sampleType == QAudioFormat::UnSignedInt)
                m_wgtAmplitude->setLevelRagne(0, 65535);
            else if (sampleType == QAudioFormat::SignedInt)
                m_wgtAmplitude->setLevelRagne(-32768, 32767);
        }
        else if (sampleSize == 32)
        {
            if (sampleType == QAudioFormat::UnSignedInt)
                m_wgtAmplitude->setLevelRagne(0, 4294967295);
            else if (sampleType == QAudioFormat::SignedInt)
                m_wgtAmplitude->setLevelRagne(-2147483648, 2147483647);
        }
        else if (sampleSize == 64)
        {
            if (sampleType == QAudioFormat::UnSignedInt)
                m_wgtAmplitude->setLevelRagne(0, 4294967295);
            else if (sampleType == QAudioFormat::SignedInt)
                m_wgtAmplitude->setLevelRagne(-9223372036854775808.0, 9223372036854775807);
        }
        else if (format.sampleType() == QAudioFormat::Float)
        {
            // TODO: needs testing on appropriate sound cards
            if (sampleType == QAudioFormat::UnSignedInt)
                m_wgtAmplitude->setLevelRagne(0, 1);
            else if (sampleType == QAudioFormat::SignedInt)
                m_wgtAmplitude->setLevelRagne(-1, 1);
        }
    }
}

void MainWindow::on_tbStop_clicked()
{
    m_audioHandler.stop();
    ChangeWorkingState(WorkState::Stopped);
}

void MainWindow::actualSampleRateEstimated(long actualSampleRate)
{
    ui->lblSampleRate->setText("Measured Sample Rate: " + QString::number(actualSampleRate));
}
