#ifndef COMMON_H
#define COMMON_H

#include <complex>
#include <memory>
#include <vector>

struct FFTAnalysisResult{
    std::shared_ptr<std::vector<std::complex<double>>> rowBuffer;
    double frequencyConversionRatio;    // frequency = *rowBuffer[i].real * frequencyConversionRatio
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

#endif // COMMON_H
