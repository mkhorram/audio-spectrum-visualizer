#include "dlgsettings.h"
#include "ui_dlgsettings.h"

DlgSettings::DlgSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSettings)
{
    ui->setupUi(this);
}

DlgSettings::~DlgSettings()
{
    delete ui;
}
