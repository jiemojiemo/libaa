//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_mir_algorithm_factory.h"
#include "libaa/mir/aa_fft.h"
#include "libaa/mir/aa_magnitude.h"
#include "libaa/mir/aa_max_filter.h"
#include "libaa/mir/aa_mir_algorithm.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
#include "libaa/mir/aa_moving_average.h"
#include "libaa/mir/aa_spectrum.h"
#include "libaa/mir/aa_superflux_novelty.h"
#include "libaa/mir/aa_superflux_peaks.h"
#include "libaa/mir/aa_triangular_bands.h"
#include "libaa/mir/aa_windowing.h"
#include <stdexcept>

namespace libaa::MIR::MIRAlgorithmFactory {
std::unique_ptr<IMIRAlgorithm> create(const std::string &name) {
    if (name == "FFT") {
        return std::unique_ptr<IMIRAlgorithm>(new FFT);
    } else if (name == "Magnitude") {
        return std::unique_ptr<IMIRAlgorithm>(new Magnitude);
    } else if (name == "MaxFilter") {
        return std::unique_ptr<IMIRAlgorithm>(new MaxFilter);
    } else if (name == "MovingAverage") {
        return std::unique_ptr<IMIRAlgorithm>(new MovingAverage);
    } else if (name == "Spectrum") {
        return std::unique_ptr<IMIRAlgorithm>(new Spectrum);
    } else if (name == "Windowing") {
        return std::unique_ptr<IMIRAlgorithm>(new Windowing);
    } else if (name == "TriangularBands") {
        return std::unique_ptr<IMIRAlgorithm>(new TriangularBands);
    } else if (name == "SuperFluxNovelty") {
        return std::unique_ptr<IMIRAlgorithm>(new SuperFluxNovelty);
    } else if (name == "SuperFluxPeaks") {
        return std::unique_ptr<IMIRAlgorithm>(new SuperFluxPeaks);
    }

    throw std::invalid_argument("cannot create " + name);
}
} // namespace libaa::MIR::MIRAlgorithmFactory