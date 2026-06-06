/// This header will handle the data types, data structures, and structural framework for the loop
/// It relies only on standard libraries and headers

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <complex>

/// Use the standard double-precision complex number type for the wavefunction
using Complex = std::complex<double>;

/// Now I will write a container for all the simulation and the physical parameters

struct SimConfig {
    int J = 1000; // The number of grid points in the lattice
    int Nt = 5000; // Total number of time steps 
    int write_interval = 50; // Export data every N steps

    double dx = 0.1; // Spatial step size
    double dt = 0.001;  // Time step size
    double mass = 1.0; // Mass of the particle
    double hbar = 1.0; // Reduced Planck's constant in natural units

    // Initilise the wavepacket
    double x0 = 30.0; // Initial position of the wavepacket
    double sigma = 5.0; // Width of the wavepacket
    double k0 = 1.0; // Initial momentum of the wavepacket

    // Potential barrier parameters
    double V0 = 1.0; // Height of the potential barrier
    double barrier_start = 50.0; // Starting position of the barrier 
    double barrier_end = 60.0; // Ending position of the barrier
};

void initialise_system(const SimConfig& config,
                        std::vector<Complex>& psi,
                        std::vector<double>& V);

void time_evolve(const SimConfig& config,
                    const std::vector<double>& V,
                    std::vector<Complex>& psi,
                    const std::string& output_file);
#endif // SIMULATION_HPP