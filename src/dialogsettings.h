#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QStringList>
#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioDeviceInfo>

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

private slots:
    void closeEvent(QCloseEvent *event);
    void on_btnRefresh_clicked();

private:
    Ui::DialogSettings *ui;

    QAudioDeviceInfo m_deviceInfo;
    QAudioFormat m_settings;
};

#endif // DIALOGSETTINGS_H
