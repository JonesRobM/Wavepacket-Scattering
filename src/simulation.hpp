/// This header will handle the data types, data structures, and structural framework for the loop
/// It relies only on standard libraries and headers

/// The main purpose of this header is to define the SimConfig struct which encapsulates all the
/// simulation parameters and to declare the functions for initializing the system and performing time evolution

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

void initialise_system(const SimConfig& config, // Pass the config struct by const reference to avoid unnecessary copying
                        std::vector<Complex>& psi, // Pass the wavefunction vector by reference to populate it with the initial wavepacket
                        std::vector<double>& V); // Pass the potential vector by reference to populate it with the potential barrier

void run_simulation(const SimConfig& config, // Pass the config struct by const reference to access the simulation parameters
                    const std::vector<double>& V, // Pass the potential vector by const reference as it is not modified during time evolution
                    std::vector<Complex>& psi, // Pass the wavefunction vector by reference to update it in place during time evolution
                    const std::string& output_file); // Pass the output file name as a const reference to avoid unnecessary copying
                    
#endif // SIMULATION_HPP