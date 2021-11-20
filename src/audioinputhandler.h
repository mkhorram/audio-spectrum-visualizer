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
#include <memory>



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

    double m_frequencyConversionRatio;
    unsigned long long m_FFTNeededSamples;
    unsigned long long m_FFTSamplesWritePoint = 0;
    std::vector<std::complex<double>> m_FFTSamples;
    std::shared_ptr<std::vector<std::complex<double>>> m_FFTOutput;

    double m_highVal = 0;
    double m_lowVal = 0;

    int m_notifyInterval;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_beginTimePoint;

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);
    void readyRead();

signals:
    void actualSampleRateEstimated(long actualSampleRate);
    void fftOutputComputed(std::shared_ptr<std::vector<std::complex<double>>> fftOutput, double ratio);
    void lowHighSampleValuesComputed(double lowInput, double highInput);

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

            if (m_format.sampleType() == QAudioFormat::Float)
                val =  qAbs(*reinterpret_cast<const float*>(ptr));
            else if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                val =  qFromLittleEndian<T>(ptr);
            else if (m_format.byteOrder() == QAudioFormat::BigEndian)
                val =  qFromBigEndian<T>(ptr);
            else
                throw "Unknown type! Type should be either LittleEndian or BigEndian.";

            if (m_highVal < val)
                m_highVal = val;
            else if (m_lowVal > val)
                m_lowVal = val;

            m_FFTSamples[m_FFTSamplesWritePoint] = val;
            ++m_FFTSamplesWritePoint;
            if (m_FFTSamplesWritePoint == m_FFTNeededSamples)
            {
                computeFFT();
                emit fftOutputComputed(m_FFTOutput, m_frequencyConversionRatio);
                m_FFTSamplesWritePoint = 0;

                emit lowHighSampleValuesComputed(m_lowVal, m_highVal);
                m_highVal = 0;
                m_lowVal = 0;
            }

            ptr += sampleBytes;
        }

        return numSamples;
    }
};

#endif // AUDIOINPUTHANDLER_H
