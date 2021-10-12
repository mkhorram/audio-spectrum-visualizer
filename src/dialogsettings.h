#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QStringList>
#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();

    void deviceChanged(int idx);

signals:
    void DialogSettingsClosed(QCloseEvent *event);

private:
    void setGlobalFormatSettings();

private slots:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

    void on_btnRefresh_clicked();
    void on_btnCancel_clicked();
    void on_btnOK_clicked();

private:
    Ui::DialogSettings *ui;

    QAudioDeviceInfo m_deviceInfo;
    QAudioFormat m_globalFormatSettings;
};

#endif // DIALOGSETTINGS_H
