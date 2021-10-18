#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>

template <typename T>
class RingBuffer
{
private:
    unsigned long m_bufSize;
    unsigned long m_readPoint {0};
    unsigned long m_writePoint {0};
    std::vector<T> m_buffer;

public:
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    RingBuffer(unsigned long bufSize = 100000) : m_bufSize(bufSize) { m_buffer.reserve(m_bufSize); }

    inline unsigned long lenghtToRead() { return (m_bufSize+m_writePoint-m_readPoint)%m_bufSize; }

    void haveRead(unsigned long length)
    {
        if (lenghtToRead() < length)
            throw "The length is beyond the available data";
        m_readPoint = (m_readPoint+length)%m_bufSize;
    }

    void insert(T& copyableVar)
    {
        if (m_bufSize - lenghtToRead() == 1)
            ++m_readPoint;
        m_buffer[m_writePoint] = copyableVar;
        ++m_writePoint;
    }

    T& operator[](unsigned long index)
    {
        if ( lenghtToRead() < index ) // how much data is available to read
            throw "The index is beyond the available data";

        unsigned long idx = (index + m_readPoint) % m_bufSize;
        return m_buffer[idx];
    }

    unsigned long getLength() const { return m_bufSize; }
};

#endif // RINGBUFFER_H
