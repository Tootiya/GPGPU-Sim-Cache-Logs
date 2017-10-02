# GPGPU-Sim-Cache-Logs
Some codes which I used through my B.Sc. thesis project "the Opportunity of Data Pre-fetching in Last-Level Cache of GPGPU under Irregular Workloads" for analyzing.
# Changed-shader-l2cache
I changed the Original code of GPGPU-sim to print some additional information about l1 and l2 caches. l1 cache code found in gpgpu-sim_distribution-master\src\gpgpu-sim\shader.cc and l2 cache code located in gpgpu-sim_distribution-master\src\gpgpu-sim\l2cache.cc.
Include Misses addresses, Access addresses and kernel, cycle number of each. (commented as tootiya or toot)
# Sequiture
An implemented form of sequitur code for analyzing misses of caches and calculating the opportunity of prefetching.
# L2-Cache-Sim
A simulation of L2 cache in C++ with parametric size and options and various replacement policies.
