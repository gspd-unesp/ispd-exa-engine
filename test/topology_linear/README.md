# Linear Topology Model

The Linear Topology Model is a simulation model that represents a linear network topology consisting of one master, **N** links, and **N** machines. This model allows users to simulate and analyze various aspects of a linear network configuration.

## Key Features

- **Linear Topology**: The model represents a linear network topology where machines are connected in a linear fashion. This topology is commonly found in various network configurations.
- **Customizable Parameters**: Users have the flexibility to specify the number of machines to be simulated and scheduled in the linear network. This enables the simulation to be tailored to specific scenarios and requirements.
- **Engine Properties**: The simulation engine provides configurable properties such as execution mode (sequential, parallel, or distributed), thread count, GVT (Global Virtual Time) interval, checkpointing interval, among others. These properties allow users to control and optimize the simulation execution.

The user can execute the model binary file and access a comprehensive list of all configurable properties by using the command-line argument **-h** or **-help**. 

## Model Overview

The linear network model consists of a master node, **N** links, and **N** machines. The machines are arranged linearly, forming a linear network topology. Each machine represents a simulated node or device in the network.

The simulation allows users to examine various aspects of the linear network, such as message passing, task scheduling, communication patterns, and system performance. By adjusting the parameters and engine properties, users can explore different scenarios and evaluate the behavior of the linear network model under various conditions.

## Model Visualization

The following figure illustrates the structure of the linear network model:

![Linear Topology Model](https://i.imgur.com/FdiarXq.png)


## Usage

To use the Linear Topology Model, follow these steps:

1. Clone the repository and navigate to the project directory.
2. Configure the model parameters and engine properties according to your requirements.
3. Build and run the simulation using the provided scripts or commands.
   ```sh
   cmake .
   make
   ./test_topology_linear
   ```
4. Analyze the simulation results and observe the behavior of the linear network model.

Please refer to the detailed documentation and examples in the repository for more information on how to use and customize the Linear Topology Model.

## Compatibility and Requirements

The Linear Topology Model is compatible with the specified simulation framework and its dependencies. Ensure that you have the required software and libraries installed and configured correctly to run the simulation.

## Contributing

Contributions to the Linear Topology Model are welcome and encouraged. If you encounter any issues, have suggestions for improvements, or would like to contribute new features, please follow the established guidelines for contributing to the project. Your contributions help improve the functionality, usability, and performance of the Linear Topology Model for the benefit of the simulation community.

Thank you for your interest in the Linear Topology Model. We appreciate your contributions and support in advancing the simulation capabilities in the field of distributed systems and analysis.

