#ifndef AUDIOINPUTHANDLER_H
#define AUDIOINPUTHANDLER_H

#include <QAudioInput>
#include <QBuffer>
#include <QVector>
#include <QAudioFormat>
#include <QIODevice>
#include <QObject>

class AudioInputHandler : public QObject
{
    Q_OBJECT
public:
    explicit AudioInputHandler();
    virtual ~AudioInputHandler() = 0;

    void setAudioFormat(QAudioFormat format);
    void setAudioDeviceInfo(QAudioDeviceInfo audioDeviceInfo);
    bool run();
    void stop();

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);

private:
    QAudioInput *m_AudioInput = nullptr;
    QBuffer  m_InputBuffer;
    QAudioFormat m_format;
    QAudioDeviceInfo m_audioDeviceInfo;

    QVector<double> m_Samples;
};

#endif // AUDIOINPUTHANDLER_H
