# Model Testing #1

The model being tested here is composed of three logical processes being them one master `M`, one link `L` and one machine `m`.

 * The topology being tested is: M -> L -> m.

 * An amount of `1.000` (a thousand) tasks is being scheduled by the master to the machine passing through the link.

Further, all generated tasks arrive at master at `0 + i * jitter` and all of the tasks have the **same** workload.

 * The `jitter` is a small amount of time, in this case, `1e-52` that is added to the time of the previous sent task. The use of jitter to define the arrival time of tasks ensure the determinism in the task arrivals and faster simulation.

