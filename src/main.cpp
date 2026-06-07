/// Initialise -> Time iterations -> Stream to disk
/// Step 1. Data structures and representations

/// The spatial domain is mapped to a uniform 1d lattice of spacing J
/// QM requires complex amplitudes. The entire state of the system at any given moment
/// can be held in standard contiguous memory

/// State vectors given by std::vector<std::complex<double>> psi_current(J)
/// The time time evolved state vector is given by std::vector<std::complex<double>> psi_next(J)

/// The potential barreier is given by a static std::vector<double> V(J)
/// This maps the potential energy value at each lattice site 


/// Inside simulation.cpp the execution will follow three phases
/// 1. Grid allocation based on the spatial step and domain size 
/// 2. Wavepacket construction: Popoulate psi_current by evlauting the modulated gassian function as xj = j delta x
/// 3. Nomralisation of the wavepacket to ensure that \sum_j |psi_current[j]|^2 = 1

/// Phase B: Time evolution 
/// A simple for loop runs from t=0 to t_final with a time step of delta t
/// At each iteration, the wavefunction is updated using the finite difference approximation of the Schrodinger equation
/// The new state psi_next is computed based on the current state psi_current and the potential V
/// After each update, psi_current is set to psi_next for the next iteration

/// Boundary conditions implemented with Dirichlet boundary conditions
/// where the wavefunction is set to zero at the boundaries of the spatial domain

/// Pointer and reference swap.
/// At the end of each time step instead of copying vectors swap their underlying data pointers via std:: swap(psi_current, psi_next) -> saves memory overheads

/// Phase C - Data export
/// Write to a file at regular intervals (every N time steps) to capture the evolution of the wavefunction and not bloat the output files with every time step
/// The file i/o format. main.cpp will open a single file stream and write a flat matrix
/// This will be written as a row per time slice where |psi_current[j]|^2 is written for each spatial point j at that time step

/// Simply solving the time dependent Schrodinger equation in one dimension with a potential barrier

/// Use the explicit forward time central space finite difference scheme as it maps well to c++

/// The governing pde is given by 
/// i hbar d psi / dt = - (hbar^2 / 2m) d^2 psi / dx^2 + V(x) psi

/// Next I need to discretise the operators such that xj = j delta x and tk = k delta t
/// The second spatial derivative can be approximated by the central difference formula
/// d^2 psi / dx^2 ~ (psi[j+1] - 2 psi[j] + psi[j-1]) / (delta x)^2
/// The time derivative can be approximated by the forward difference formula
/// d psi / dt ~ (psi_next[j] - psi_current[j]) / delta t
/// The update formula is then given by
/// psi_next[j] = psi_current[j] + (i hbar delta t / 2m) (psi_current[j+1] - 2 psi_current[j] + psi_current[j-1]) / (delta x)^2 - (i delta t / hbar) V[j] psi_current[j]
/// A clean way of doing this in c++ is to isolate  tthe real and imaignary parts by factoring out i.
/// I will define the grouping parameter alpha as alpha = hbar delta t / (2m (delta x)^2) and beta as beta = delta t / hbar
/// This cleans the update formula to
/// psi_next[j] = psi_current[j] + i alpha (psi_current[j+1] - 2 psi_current[j] + psi_current[j-1]) - i beta V[j] psi_current[j]

/// WARNING - The explicit forward time central space scheme is conditionally stable and may require small time steps to ensure numerical stability
/// The stability condition is given by alpha <= 0.5, which translates to delta t <= (delta x)^2 m / hbar
/// Careful selection of delta t and delta x is crucial to avoid numerical instabilities in the simulation
/// This is known as the Courant-Friedrichs-Lewy (CFL) condition for the Schrodinger equation and must be adhered to for accurate and stable simulations

/// The output will be given as a single file with rows corresponding to time steps and columns corresponding to spatial points
/// Where each entry is the probability density |psi_current[j]|^2 at that point in space and time
/// This format allows for easy post-processing and visualization of the wavefunction evolution over time
/// Every mth time step the loop will iterate over a spatial array from j - 0 to J-1 and dump a row of text

#include "simulation.hpp"
#include <iostream>
#include <string>

int main() {
    std:: cout << "Initialising the super-duper 1d quantum wavepacket FDTD engine ...\n";

    SimConfig config; // Create an instance of the SimConfig struct to hold all the simulation parameters

    // Calculate the stability parameter alpha to check if the chosen time step and spatial step satisfy the stability condition for the explicit finite difference scheme
    double stability_check = ( config.hbar * config.dt ) / (2.0 * config.mass * std::pow(config.dx, 2)); 
    
    if (stability_check > 0.5) {
        std::cerr << "Warning: The chosen time step and spatial step may lead to numerical instability. Consider reducing dt or increasing dx to satisfy the stability condition.\n"
                    << "Alpha = " << stability_check << " should be less than or equal to 0.5 for stability.\n";
        return 1; // Exit the program with an error code to indicate that the parameters are not suitable for a stable simulation
    }

    std::vector<Complex> psi;
    std::vector<double> V;

    // Call the function to initialize the wavefunction and potential based on the configuration parameters
    initialise_system(config, psi, V); 

    std::string data_path = "output/simulation_output.csv"; // Define the path for the output file where the simulation results will be written
    std::cout << "Beginning numerical time evolution. Writing results to " << data_path << "\n";

    run_simulation(config, V, psi, data_path);

    std::cout << "Simulation complete. Pipeline terminated cleanly. Results written to " << data_path << "\n";
    return 0; // Exit the program with a success code to indicate that the simulation completed successfully
}