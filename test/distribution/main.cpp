#include <allocator/rootsim_allocator.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <model/builder.hpp>
#include <service/dummy.hpp>
#include <simulator/simulator.hpp>
#include <tclap/ArgException.h>
#include <tclap/CmdLine.h>
#include <workload/distribution.hpp>

using namespace ispd::sim;
using namespace std::chrono;

static inline uint64_t getSeed()
{
    return duration_cast<nanoseconds>(
               high_resolution_clock::now().time_since_epoch())
        .count();
}

int main(int argc, char **argv)
{
    try {
        /// Construct the command-line parser.
        TCLAP::CmdLine cmd("Arrival Time Distribution", ' ', "v0.0.1");

        /// Argument to specify the sample size to be generated.
        TCLAP::ValueArg<uint32_t> sampleSizeArg(
            "n",
            "sample-size",
            "Specify the amount of task arrival time to be generated.",
            false,
            100,
            "uint32_t");
        cmd.add(sampleSizeArg);

        /// Argument to specify the interarrival probability distribution.
        TCLAP::ValueArg<std::string> distributionTypeArg(
            "d",
            "distribution",
            "Specify the interarrival probability distribution.",
            false,
            "poisson",
            "string");
        cmd.add(distributionTypeArg);

        /// Argument to specify the PRNG Seed.
        TCLAP::ValueArg<uint64_t> prngSeedArg(
            "s",
            "prng-seed",
            "Specify the Pseudorandom Number Generator (PRNG) Seed",
            false,
            getSeed(),
            "uint64_t");
        cmd.add(prngSeedArg);

        /// Argument to specify the mean argument for intearrival probability
        /// density functions that use the mean parameter.
        TCLAP::ValueArg<double> meanArg(
            "m",
            "mean",
            "Specify the mean parameter for interarrival probability "
            "distribution that use it.",
            false,
            1.0,
            "double");
        cmd.add(meanArg);

        /// Argument to specify whether the debugging should be enabled.
        TCLAP::SwitchArg debugArg(
            "l",
            "debug",
            "Enable whether the distribution should be debugged.",
            false);
        cmd.add(debugArg);

        /// Parse the command-line arguments.
        cmd.parse(argc, argv);

        const uint32_t sampleSize   = sampleSizeArg.getValue();
        const double   mean         = meanArg.getValue();
        const bool     debugEnabled = debugArg.getValue();

        const std::string filepath = "distribution/distribution.txt";
        std::ofstream     outputFile(filepath);

        /// Checks if the file could not be opened. If so, the
        /// program is immediately aborted.
        if (!outputFile.is_open())
            die("File `%s` could not be opened.", filepath.c_str());

        /// Just create a simulator object, there is no need for additional
        /// configuration in this case.
        Simulator *s = SimulatorBuilder(ispd::sim::SimulatorType::ROOTSIM,
                                        SimulationMode::OPTIMISTIC)
                           .setPrngSeed(prngSeedArg.getValue())
                           .createSimulator();

        /// Register the dummy service.
        s->registerService(
            0ULL, [sampleSize, mean, debugEnabled, &outputFile]() {
                double lastArrivalTime = 0.0;
                double arrivalTime;
                double interarrivalTime;

                WorkloadDistribution *wd =
                    new PoissonWorkloadDistribution(mean);

                for (int i = 0; i < sampleSize; i++) {
                    wd->setArrivalTime(arrivalTime);

                    interarrivalTime = arrivalTime - lastArrivalTime;
                    lastArrivalTime  = arrivalTime;

                    /// Checks if the debug is enabled. If so, the arrival time
                    /// and the intearrival time will be printed out to the
                    /// standard output.
                    if (debugEnabled) {
                        std::printf(
                            "Arrival Time: %lf, Interarrival Time: %lf.\n",
                            arrivalTime,
                            interarrivalTime);
                    }

                    outputFile << arrivalTime << ", " << interarrivalTime
                               << '\n';
                }

                /// Close the file.
                outputFile.close();

                /// Construct a dummy service.
                return ROOTSimAllocator<>::construct<Dummy>(0ULL);
            });

        /// @Note: The output file in which the distribution sample will be
        ///        written into should not be closed in here, since the dummy
        ///        service that will print the sample will be executed after
        ///        this part of code.

        /// Execute the simulation.
        s->simulate();
    }
    catch (const TCLAP::ArgException &e) {
        std::cerr << "Error " << e.error() << " in argument " << e.argId()
                  << "." << std::endl;
    }

    return 0;
}
