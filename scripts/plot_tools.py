"""
This module will contian the functions necessary for reading and 
plotting the results of the simulations

We will use matplotlib for plotting and numpy for data manipulation

The main functions will be:

- load_simulation_data: This function will read the output data from the simulation and return it in a format suitable for plotting
- plot_wavefunction: This function will take the wavefunction data and plot it as a function of position and time
"""

import numpy as np
import matplotlib.pyplot as plt
import os



def load_simulation_data(file_path: str) -> np.ndarray:
    """_summary_
    returns:
        np.ndarray:
        A 2d array of shape (num_time_steps, num_grid_points)
        containing the wavefunction data at each time step and grid point
        Represents the probability density of the wavefunction at each point in space and time
    """
    return np.loadtxt(file_path, delimiter=',')

def reconstruct_axes(
    matrix: np.ndarray,
    dx: float,
    dt: float,
    write_interval: int) -> tuple[np.ndarray, np.ndarray]:
    """
    Rcereates the independent coordinate vectors in physical units
    
    args:
        matrix (np.ndarray): The 2D array containing the wavefunction data
        dx (float): The spatial grid spacing used in the simulation
        dt (float): The time step used in the simulation
        write_interval (int): The interval at which the simulation data was written to file
    
    returns:
        tuple[np.ndarray, np.ndarray]: A tuple containing the x-axis and t-axis coordinate vectors
    """
    
    num_time_slices, num_spatial_points = matrix.shape
    
    x_axis =np.arange(num_spatial_points) * dx
    
    effective_dt = dt * write_interval
    t_axis = np.arange(num_time_slices) * effective_dt
    
    return x_axis, t_axis

def verify_probability_conservation(
    matrix: np.ndarray, dx: float) -> np.ndarray:
    """
    
    Integrates the probability density over space for each time step
    Mathematically it evlauate s I(t) = p(x,t)dx ~ sum P_j dx
    where P_j is the probability density at grid point j and time t
    
    returns:
        np.ndarray: A 1D array containing the total probability at each time step
        Ideally very close to a unit vector
    """
    
    return np.trapezoid(matrix, dx = dx, axis=1)

def calculate_expected_position(
    matrix: np.ndarray, x_axis: np.ndarray, dx: float) -> np.ndarray:
    """
    Calculates the expected position of the wavefunction at each time step
    Mathematically it evaluates <x>(t) = int x p(x,t) dx ~ sum x_j P_j dx
    where x_j is the position of grid point j and P_j is the probability density at that point and time
    
    returns:
        np.ndarray: A 1D array containing the expected position at each time step
    """
    weighted_probability = matrix * x_axis
    return np.trapezoid(weighted_probability, dx = dx, axis=1)

def plot_wavefunction(
    matrix: np.ndarray,
    x_axis: np.ndarray,
    t_axis: np.ndarray,
    output_dir: str) -> None:
    """
    Plots the wavefunction as a function of position and time
    
    args:
        matrix (np.ndarray): The 2D array containing the wavefunction data
        x_axis (np.ndarray): The spatial coordinate vector
        t_axis (np.ndarray): The time coordinate vector
        output_dir (str): The directory where the plot will be saved
    
    returns:
        None: The function saves the plot to the specified directory
    """
    
    plt.figure(figsize=(10,6))
    plt.imshow(matrix, extent=[x_axis[0], x_axis[-1], t_axis[-1], t_axis[0]], aspect='auto', cmap='viridis')
    plt.colorbar(label='Probability Density')
    plt.xlabel('Position (a.u.)')
    plt.ylabel('Time (a.u.)')
    
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, 'wavefunction_evolution.png')
    plt.savefig(output_path)
    plt.close()
    
def plot_expected_position(
    expected_position: np.ndarray,
    t_axis: np.ndarray,
    output_dir: str) -> None:
    """
    Plots the expected position of the wavefunction as a function of time
    
    args:
        expected_position (np.ndarray): A 1D array containing the expected position at each time step
        t_axis (np.ndarray): The time coordinate vector
        output_dir (str): The directory where the plot will be saved
    
    returns:
        None: The function saves the plot to the specified directory
    """
    
    plt.figure(figsize=(10,6))
    plt.plot(t_axis, expected_position, label='Expected Position')
    plt.xlabel('Time (a.u.)')
    plt.ylabel('Expected Position (a.u.)')
    plt.legend()
    
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, 'expected_position.png')
    plt.savefig(output_path)
    plt.close()