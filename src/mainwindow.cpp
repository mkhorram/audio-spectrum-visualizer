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
    ui->toolBar->addWidget(ui->tbPause);
    ui->toolBar->addWidget(ui->tbStop);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    m_wgtAmplitude = new WidgetAmplitude(this);
    m_wgtFrequencyViewer = new WidgetFrequencyViewer(this);
    splitter->addWidget(m_wgtAmplitude);
    splitter->addWidget(m_wgtFrequencyViewer);
    splitter->setStretchFactor(0,4);
    splitter->setStretchFactor(1,1);
    this->setCentralWidget( splitter );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tbSettings_clicked()
{
    (m_dlgSettings.isVisible())?  m_dlgSettings.hide() : m_dlgSettings.show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_dlgSettings.isVisible())
        m_dlgSettings.close();
    event->accept();
}
