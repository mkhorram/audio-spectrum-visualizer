#ifndef AUDIOINPUTHANDLER_H
#define AUDIOINPUTHANDLER_H

#include <QAudioInput>
#include <QBuffer>
#include <QVector>
#include <QAudioFormat>
#include <QIODevice>
#include <QObject>
#include <QtEndian>

#include <chrono>
#include <complex>
#include <vector>



class AudioInputHandler : public QObject
{
    Q_OBJECT
private:
    QAudioInput *m_AudioInput = nullptr;
    QIODevice * IODevice;
    QAudioFormat m_format;
    QAudioDeviceInfo m_audioDeviceInfo;
    int m_channel = 0;

    unsigned long long m_frequencyNeededSamples;
    unsigned long long m_frequencySampleCount;

    const unsigned long long m_buf_length;
    char * m_buf;

    unsigned long long m_FFTNeededSamples;
    unsigned long long m_FFTSamplesWritePoint = 0;
    std::vector<std::complex<double>> m_FFTSamples;
    std::vector<std::complex<double>> m_FFTOutput;

    double m_maxVal = 0;
    double m_minVal = 0;

    int m_notifyInterval;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_beginTimePoint;

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);
    void readyRead();

signals:
    void actualSampleRateEstimated(long actualSampleRate);
    void fftOutputComputed(const std::vector<std::complex<double>> &fftOutput);
    void minMaxSampleValuesComputed(double minVal, double maxVal);

public:
    explicit AudioInputHandler(unsigned long long buf_length = 100000);
    ~AudioInputHandler();

    bool start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo, unsigned long long FFTNeededSamples, unsigned long long frequencyNeededSamples);
    void stop();

private:
    void checkActualSampleRate(unsigned long long receivedBytesCount);
    inline void computeFFT();

    template <typename T>
    long castDataToDouble(char * data, long len)
    {
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const long numSamples = len / sampleBytes;

        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
        ptr += (m_channel * channelBytes);

        for (int i = 0; i < numSamples; ++i)
        {
            double val = 0;
            if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                val =  qFromLittleEndian<T>(ptr);
            else if (m_format.byteOrder() == QAudioFormat::BigEndian)
                val =  qFromBigEndian<T>(ptr);
            else
                throw "Unknown type! Type should be either LittleEndian or BigEndian.";

            if (m_maxVal < val)
                m_maxVal = val;
            else if (m_minVal > val)
                m_minVal = val;

            m_FFTSamples[m_FFTSamplesWritePoint] = val;
            ++m_FFTSamplesWritePoint;
            if (m_FFTSamplesWritePoint == m_FFTNeededSamples)
            {
                computeFFT();
                emit fftOutputComputed(m_FFTOutput);
                m_FFTSamplesWritePoint = 0;

                emit minMaxSampleValuesComputed(m_minVal, m_minVal);
                m_maxVal = 0;
                m_minVal = 0;
            }

            ptr += sampleBytes;
        }

        return numSamples;
    }
};

#endif // AUDIOINPUTHANDLER_H
