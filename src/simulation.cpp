/// This file performs the numerical heavy lifting
/// It computes the initial gaussian state and exectutes the time evolution loop

#include "simulation.hpp" // This is our custom header for the simulation framework
#include <fstream> // For file output
#include <cmath> // For mathematical functions like exp and pow
#include <utility> // For std::swap to swap vectors without copying

void initialise_system(
                        const SimConfig& config,
                        std::vector<Complex>& psi,
                        std::vector<double>& V) {
    psi.resize(config.J); // Resize the wavefunction vector to match the number of grid points
    V.resize(config.J, 0.0); // Resize the potential vector and initialize it to zero

    double norm_sum = 0.0; // Variable to accumulate the normalization factor

    // 1. Populate the wavefunction and potential grid
    for (int j = 0; j< config.J; ++j) {
        double x = j * config.dx; // Calculate the spatial position corresponding to the grid point j. We get dx from the config struct which defines the spatial step size
    }
}