#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <cmath>

#include <QDebug>

template <typename T>
class RingBuffer
{
private:
    long long m_bufSize;
    long long m_writePoint {0};
    long long m_readableLength {0};
    std::vector<T> m_buffer;

public:
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    RingBuffer(unsigned long bufSize) : m_bufSize(bufSize) { m_buffer.resize(m_bufSize); }

    long getBufSize() const { return m_bufSize; }

    inline unsigned long getlenghtToRead() {
        return m_readableLength;
    }

    void moveReadPoint(unsigned long length)
    {
        if (m_readableLength < length)
            throw "The length, to be removed, is beyond the available data.";
        m_readableLength -= length;
        if (m_readableLength < 0)
            m_readableLength = 0;
    }

    T& operator[](long index)
    {
        if (index > m_readableLength || index < 0) // how much data is available to read
            throw "The index is beyond the available data.";
        unsigned long idx = (index + m_writePoint - m_readableLength + m_bufSize) % m_bufSize;

        return m_buffer[idx];
    }

    void insert(T& copyableVar)
    {
        m_buffer[m_writePoint] = copyableVar;
        m_writePoint = (++m_writePoint) % m_bufSize;
        if (m_readableLength < m_bufSize)
            m_readableLength++;
    }

    void insert(std::vector<T> copyableVarVector)
    {
        for (int i = 0; i < copyableVarVector.size(); ++i)
            this->insert(copyableVarVector[i]);
    }

    void copyToVector(std::vector<T> &varVector, unsigned int fromIndex, unsigned int length)
    {
        for (int i = 0; i < length; ++i)
            varVector.push_back(this[fromIndex+i]);
    }

    void reset()
    {
        m_readableLength = 0;
        m_writePoint = 0;
    }
};

#endif // RINGBUFFER_H
