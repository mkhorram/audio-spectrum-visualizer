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

private:
    void measureActualSampleRate(long long newReceivedSampleCount);

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

    long m_minSampleCount;
    long m_sampleCount;

    const long long m_buf_length = 100000;
    char * m_buf;
    std::vector<double> m_samples;
    int m_notifyInterval;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_timePoint;
};

#endif // AUDIOINPUTHANDLER_H
