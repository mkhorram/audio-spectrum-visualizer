#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
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

private slots:
    void on_btnRefresh_clicked();

private:
    Ui::DialogSettings *ui;
};

#endif // DIALOGSETTINGS_H
