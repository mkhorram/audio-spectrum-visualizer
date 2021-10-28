#ifndef AUDIOINPUTHANDLER_H
#define AUDIOINPUTHANDLER_H

#include <QAudioInput>
#include <QBuffer>
#include <QVector>
#include <QAudioFormat>
#include <QIODevice>
#include <QObject>

#include <chrono>

class AudioInputHandler : public QObject
{
    Q_OBJECT
public:
    explicit AudioInputHandler();
    ~AudioInputHandler();

    bool start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo);
    void stop();

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);
    void readyRead();

private:
    QAudioInput *m_AudioInput = nullptr;
    QIODevice * IODevice;
    QAudioFormat m_format;
    QAudioDeviceInfo m_audioDeviceInfo;

    QVector<double> m_Samples;
    int m_notifyInterval;

    const unsigned long long m_buf_length = 100000;
    char * m_buf;
    std::chrono::time_point<std::chrono::high_resolution_clock> tp;
};

#endif // AUDIOINPUTHANDLER_H
