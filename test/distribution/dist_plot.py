import os
import sys
import matplotlib.pyplot as plt

# Checks if the user has not specified the distribution file path.
# If no file path is provided as a command-line argument, the program will exit immediately.
if len(sys.argv) == 1:
    # Prints an error message indicating that the distribution file path has not been specified.
    print('Error: Distribution file path has not been specified.')

    # Exits the program with a status code of 0 (indicating a successful termination).
    exit(0)

# Get the filepath from the command-line arguments.
# The filepath should be provided as the second argument (index 1) in the command-line.
filepath = sys.argv[1]

# Checks if the specified filepath does not exist.
# If the file does not exist at the provided filepath, the program will exit immediately.
if not os.path.exists(filepath):
    # Prints an error message indicating that the distribution filepath does not exist.
    print('Error: Distribution filepath does not exist.')

    # Exits the program with a status code of 0 (indicating a successful termination).
    exit(0)

# A vector (list) that will contain the arrival times read from the specified filepath.
arrival = []

# A vector (list) that will contain the interarrival times read from the specified filepath.
interarrival = []

# Open the file at the specified filepath in read mode.
with open(filepath, 'r') as f:
    # Iterate over each line in the file.
    for line in f:
        # Split each line using the delimiter ',' to obtain the arrival time and interarrival time.
        arrival_time, interarrival_time = map(float, line.rstrip().split(','))

        # Append the arrival time to the arrival list.
        arrival.append(arrival_time)

        # Append the interarrival time to the interarrival list.
        interarrival.append(interarrival_time)

# Plot the histogram of the arrival times.
plt.subplot(1, 2, 1)
plt.plot(range(0, len(arrival)), arrival)

# Plot the histogram of the interarrival times.
plt.subplot(1, 2, 2)
plt.hist(interarrival, bins=30)

plt.show()

