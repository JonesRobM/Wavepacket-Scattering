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
    // What we start with is picking up J from the header and looping over the spatial grid points to populate the wavefunction and potential
    for (int j = 0; j< config.J; ++j) {
        double x = j * config.dx; // Calculate the spatial position corresponding to the grid point j. We get dx from the config struct which defines the spatial step size

        // Now we build the potential barrier. We check if the position x falls within the defined barrier region and set the potential accordingly
        if ( x>= config.barrier_start && x <= config.barrier_end) {
            V[j] = config.V0; // Set the potential at grid point j to the barrier height V0 if it falls within the barrier region
        }
        // Now we setup the un-nomrlaised gasusian wavepacket
        // We calculate the amplitude of the wavefunction at each grid point using the modulated gaussian formula
        //  exp(- (x - x0)^2 / (2*sigma^2)) * exp(i * k0 * x) where x0 is the initial position of the wavepacket and sigma is its width
        double phase = config.k0 * x; // Calculate the phase of the wavefunction at position x based on the initial momentum k0
        Complex spatial_part = std::exp(-std::pow(x - config.x0, 2) / (2.0 * std::pow(config.sigma,2))); // Calculate the spatial part of the wavefunction using the Gaussian formula
        Complex momentum_part  = Complex(std::cos(phase), std::sin(phase)); // Calculate the momentum part of the wavefunction using Euler's formula to convert the phase into a complex exponential
        
        psi[j] = spatial_part * momentum_part; // Combine the spatial and momentum parts to get the un-normalized wavefunction at grid point j
        norm_sum += std::norm(psi[j]) * config.dx; // Accumulate the normalization factor by summing the squared magnitude of the wavefunction at each grid point multiplied by the spatial step size
    }

    // 2. Normalise the wavefunction
    double  norm_factor = std::sqrt(norm_sum); // Calculate the normalization factor by taking the square root of the accumulated norm sum
    for (int j =0; j < config.J; ++j) {
        psi[j] /= norm_factor; // Normalize the wavefunction at each grid point by dividing it by the normalization factor
    }
}

void run_simulation(const SimConfig& config, // We pass the configuration struct to the simulation function, which contains all the parameters needed for the simulation
                    const std::vector<double>& V, // This initialises the potential grid which we will use in the time evolution loop
                    std::vector<Complex>& psi_current, // This is the current state of the wavefunction which will be updated at each time step
                    const std::string& output_file) { // The name of the output file where we will write the results of the simulation
                
                    std::ofstream out_file(output_file); // Open the output file for writing. This will create the file if it doesn't exist or overwrite it if it does
                    std::vector<Complex> psi_next(config.J, 0.0); // Create a vector to hold the next state of the wavefunction, initialized to zero

                    // Pre-calculate the dimensionless constant groups to reduce the loop arithmetic
                    double alpha = (config.hbar * config.dt) / (2.0 * config.mass * std::pow(config.dx, 2));
                    double beta = (config.dt / config.hbar);
                    const Complex i(0.0, 1.0); // Define the imaginary unit as a complex number
                
                    // Now we do some time looping
                    for (int n = 0; n < config.Nt; n++) {
                        for (int j = 0; j < config.J; ++j) {
                            // Write the probability density at each grid point to the output file, separated by commas
                            //We check if it's the last grid point to avoid adding a comma at the end of the line
                
                            out_file << std::norm(psi_current[j]) << (j == config.J -1 ? "" : ","); 
                        }
                        out_file << "\n"; // Add a newline after writing the probability density for all grid points at the current time step
                    }
                    
                }