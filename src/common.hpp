#ifndef COMMON_H
#define COMMON_H

#include <complex>
#include <memory>
#include <vector>

struct FFTAnalysisResult{
    std::shared_ptr<std::vector<std::complex<double>>> rowBuffer; // freq Amplitude = *rowBuffer[i].real?
    double frequencyConversionRatio;    // freq = i * frequencyConversionRatio
    FFTAnalysisResult& operator=(const FFTAnalysisResult& source)
    {
        rowBuffer = source.rowBuffer;
        frequencyConversionRatio = source.frequencyConversionRatio;
        return *this;
    }
};

enum class FFTAmplitudeToPixelMixingType
{
    Average,
    SquareAverage,
    MaximumValue
};

struct FFTRangeToPixelMap
{
    int pixCol = 0;
    int pixelCount = 0;
    long subFFTRangeStart = 0;
    long subFFTRangeLength = 0;
};

#endif // COMMON_H
