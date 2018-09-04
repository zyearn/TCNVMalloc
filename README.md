# TCNVMalloc

## What is tcnvmalloc

we propose Wamalloc(alias for tcncmalloc), a wear-aware allocator that provide wear-leveling without degrading the performance of allocator when compared with the state-of-the-art allocators. Specificly, this paper introduces three main techniques of NVM allocator. First, we propose a novel hybrid(fine-grained and coarse-grained) wear-leveling policy proven to be more accurate; Besides, we use a thread-cache structure and lock optimizations that result in a better performance compared with other NVM and DRAM allocators. At last, Wamalloc decouples metadata and data management, since putting metadata into DRAM will decrease plenty of writes to NVM, which will probably extend the lifetime of PCM.

Our contributions can be summarized as follows:
* We propose Wamalloc, which (1) uses a hybrid wear-leveling policy focusing on an accurate wear-leveling policy; (2) has a good performance compared with the state-of-the-art NVM and DRAM allocators; (3) decouples metadata and data management to reduce huge small writes.
* We implement Wamalloc as an userspace library, which depends on no special requirements on operating system and hardware modifications.
* We implement a prototype of Wamalloc and evaluate its performance and some critical aspects against several memory allocators. Experiment shows that under both uniform and random workloads, Wamalloc outperforms the prior allocators in aspect of average block allocation frequency, total memory consumption and allocation latency.

## paper

Zhu J, Li S, Huang L. Wamalloc: An Efficient Wear-Aware Allocator for Non-Volatile Memory[C]//2016 IEEE 22nd International Conference on Parallel and Distributed Systems (ICPADS). IEEE, 2016: 625-634.
