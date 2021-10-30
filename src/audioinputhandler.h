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
private:
    QAudioInput *m_AudioInput = nullptr;
    QIODevice * IODevice;
    QAudioFormat m_format;
    QAudioDeviceInfo m_audioDeviceInfo;
    int m_channel = 0;

    long m_minSampleCount;
    long m_sampleCount;

    const long long m_buf_length = 100000;
    char * m_buf;
    std::vector<double> m_samples;
    int m_notifyInterval;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_timePoint;

public:
    explicit AudioInputHandler();
    ~AudioInputHandler();

    bool start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo);
    void stop();

private:
    void measureActualSampleRate(long long newReceivedSampleCount);

    template <typename T>
    void castDataToDouble(char * data, long len)
    {
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const long numSamples = len / sampleBytes;

        double maxVal = 0;
        double minVal = 0;

        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
        ptr += (m_channel * channelBytes);

        for (int i = 0; i < numSamples; ++i)
        {
            double val =  *reinterpret_cast<T>(ptr);
            if (maxVal < val)
                maxVal = val;
            else if (minVal > val)
                minVal = val;
            m_samples[i] = val;

            ptr += sampleBytes;
        }
    }

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);
    void readyRead();
};

#endif // AUDIOINPUTHANDLER_H
