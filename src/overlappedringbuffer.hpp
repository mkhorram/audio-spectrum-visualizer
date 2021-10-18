#ifndef OVERLAPPEDRINGBUFFER_H
#define OVERLAPPEDRINGBUFFER_H

#include <vector>

class OverlappedRingBuffer
{
private:
    unsigned long m_bufSize;
    unsigned long m_overlapSize;
    unsigned long m_readPoint {0};
    unsigned long m_writePoint {0};

public:
    OverlappedRingBuffer(unsigned long bufSize, unsigned long overlapSize) : m_bufSize(bufSize), m_overlapSize(overlapSize) {}
    OverlappedRingBuffer() : m_bufSize(100000), m_overlapSize(5000) {}

};

#endif // OVERLAPPEDRINGBUFFER_H
