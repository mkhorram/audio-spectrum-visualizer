#ifndef AUDIOINPUTHANDLER_H
#define AUDIOINPUTHANDLER_H

#include <QAudioInput>
#include <QBuffer>
#include <QVector>
#include <QAudioFormat>
#include <QIODevice>
#include <QObject>

class AudioInputHandler
{
public:
    AudioInputHandler();
    ~AudioInputHandler();

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);

private:
    QAudioInput *m_AudioInput = nullptr;
    QBuffer  m_InputBuffer;

    QVector<double> m_Samples;
};

#endif // AUDIOINPUTHANDLER_H
