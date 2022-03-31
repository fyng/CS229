// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104

# Contributions #
*AS3MS1:*
We created the make file and downloaded all the trace files to be used. We also have the beginnings of the argument line and reading files as well.

*AS3MS2:*
Tae Wan implemented the control flow for cache store and load
Feiyang implemented input and string parsing
Both debugged together

*AS3MS3:*
Feiyang redesigned the OO, implemented FIFO and corrected statistics for load/store hit/miss.
Tae Wan debugged and did the majority of code refactoring.

=============

# Best Cache: #
*Experiment 1: optimize cache size allocation*
For the first experiment, we adopt the write-back + write-allocate policy, with LRU eviction. The cache size is fixed at 1MB (2^20). The parameters varied are number of sets, blocks per set, bytes per block.  We choose 3 values for bytes per block, then traded off the number of sets with the blocks per set under the constraint of 1MB cache size. The results are obtained using gcc.trace and summarized in the table below. 

| sets   | blocks\_per\_set | bytes\_per\_block (min 4) | total load | total store | load hit | load miss | store hit | store miss | total cycles            |
| ------ | ---------------- | ------------------------- | ---------- | ----------- | -------- | --------- | --------- | ---------- | ----------------------- |
| 262144 | 1                | 4                         | 318197     | 197486      | 313747   | 4450      | 170121    | 27365      |               3,700,583 |
| 65536  | 4                | 4                         | 318197     | 197486      | 313765   | 4432      | 170129    | 27357      |               3,694,783 |
| 16384  | 16               | 4                         | 318197     | 197486      | 313765   | 4432      | 170130    | 27356      |               3,694,483 |
| 4096   | 64               | 4                         | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      |               3,694,383 |
| 1024   | 256              | 4                         | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      |               3,694,383 |
| 256    | 1024             | 4                         | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      |               3,694,383 |
| 64     | 4096             | 4                         | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      |               3,694,383 |
| 16     | 16384            | 4                         |            |             |          |           |           |            |                         |
| 4      | 65536            | 4                         |            |             |          |           |           |            |                         |
| 1      | 262144           | 4                         | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      |               3,694,383 |
|        |                  |                           |            |             |          |           |           |            |                         |
| sets   | blocks\_per\_set | bytes\_per\_block (min 4) | total load | total store | load hit | load miss | store hit | store miss | total cycles            |
| 16384  | 1                | 64                        | 318197     | 197486      | 317169   | 1028      | 195094    | 2392       |               6,070,883 |
| 4096   | 4                | 64                        | 318197     | 197486      | 317201   | 996       | 195109    | 2377       |               5,922,083 |
| 1024   | 16               | 64                        | 318197     | 197486      | 317203   | 994       | 195112    | 2374       |               5,918,883 |
| 256    | 64               | 64                        | 318197     | 197486      | 317204   | 993       | 195112    | 2374       |               5,902,883 |
| 64     | 256              | 64                        | 318197     | 197486      | 317204   | 993       | 195112    | 2374       |               5,902,883 |
| 16     | 1024             | 64                        |            |             |          |           |           |            |                         |
| 4      | 4096             | 64                        |            |             |          |           |           |            |                         |
| 1      | 16384            | 64                        | 318197     | 197486      | 317204   | 993       | 195112    | 2374       |               5,902,883 |
|        |                  |                           |            |             |          |           |           |            |                         |
| sets   | blocks\_per\_set | bytes\_per\_block (min 4) | total load | total store | load hit | load miss | store hit | store miss | total cycles            |
| 1024   | 1                | 1024                      | 318197     | 197486      | 317661   | 536       | 197111    | 375        |            28,240,483   |
| 256    | 4                | 1024                      | 318197     | 197486      | 317921   | 276       | 197228    | 258        |            14,877,283   |
| 64     | 16               | 1024                      | 318197     | 197486      | 317928   | 269       | 197230    | 256        |            14,339,683   |
| 16     | 64               | 1024                      | 318197     | 197486      | 317931   | 266       | 197230    | 256        |            13,878,883   |
| 4      | 256              | 1024                      | 318197     | 197486      | 317931   | 266       | 197230    | 256        |            13,878,883   |
| 1      | 1024             | 1024                      | 318197     | 197486      | 317931   | 266       | 197230    | 256        |            13,878,883   |

To evaluate the results, we focus on the cycle count for performance evaluation as this directly correlates with the time taken for memory operations. Firstly, smaller bytes per block clearly led to lower cycle counts. Therefore, we conclude that the optimal number of bytes per block is 4. In reality, this number will be higher (e.g. 128 bytes) to accommodate the size needed for memory reference. 

We observe that a higher associativity leads to lower cycle count, which means better performance. However, there is diminishing return for higher associativity. The same performance can be achieved with an associativity factor of 64 for all of our experiments. Considering that higher associativity factor makes the cache more difficult to engineer in reality due to a higher number of parallel comparisons that must be made, we conclude that the optimal associativity factor is 64. We also acknowledge that in reality, even lower associativity factor (e.g. 16) might be desirable as it achieves most of the performance while making the engineering of the cache much easier. 

Therefore, we conclude that the optimal parameters for a 1MB write-back + write-allocate cache with LRU eviction policy is:
Number of sets: 4096
Number of blocks per set: 64
Number of bytes per block: 4

The pattern observed with gcc.trace is sanity-checked using swim.trace on a coarser search grid.

| sets   | blocks\_per\_set | bytes\_per\_block (min 4) | total load | total store | load hit | load miss | store hit | store miss | total cycles |
| ------ | ---------------- | ------------------------- | ---------- | ----------- | -------- | --------- | --------- | ---------- | ------------ |
| 262144 | 1                | 4                         | 220668     | 82525       | 219145   | 1523      | 61674     | 20851      | 2541793      |
| 65536  | 4                | 4                         | 220668     | 82525       | 219148   | 1520      | 61677     | 20848      | 2539993      |
| 4096   | 64               | 4                         | 220668     | 82525       | 219148   | 1520      | 61677     | 20848      | 2539993      |
|        |                  |                           |            |             |          |           |           |            |              |
| sets   | blocks\_per\_set | bytes\_per\_block (min 4) |            |             |          |           |           |
| 16384  | 1                | 64                        | 220668     | 82525       | 220232   | 436       | 79430     | 3095       | 6042393      |
| 256    | 64               | 64                        | 220668     | 82525       | 220275   | 393       | 79465     | 3060       | 5827993      |
| 1      | 16384            | 64                        | 220668     | 82525       | 220275   | 393       | 79465     | 3060       | 5827993      |
|        |                  |                           |            |             |          |           |           |            |              |
| sets   | blocks\_per\_set | bytes\_per\_block (min 4) |            |             |          |           |           |
| 1024   | 1                | 1024                      | 220668     | 82525       | 220401   | 267       | 82219     | 306        | 17890393     |
| 16     | 64               | 1024                      | 220668     | 82525       | 220560   | 108       | 82299     | 226        | 8853593      |
| 1      | 1024             | 1024                      | 220668     | 82525       | 220560   | 108       | 82299     | 226        | 8853593      |


*Experiment 2: optimize cache policy*
For the second experiment, we a 1MB cache with the optimal parameters listed above. We then experimented with a combination of cache policies. 

Testing on both gcc.trace and swim.trace consistently showed that write-allocate + write-back policy consistently performed about an order of magnitude better than alternatives. 
LRU and FIFO both generated the same cycle count and does not appear to be a significant factor. 

gcc.trace tests:
| write-miss policy | write-hit policy | eviction policy | total load | total store | load hit | load miss | store hit | store miss | total cycles |
| ----------------- | ---------------- | --------------- | ---------- | ----------- | -------- | --------- | --------- | ---------- | ------------ |
| write-allocate    | write-back       | lru             | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      | 3694383      |
| write-allocate    | write-through    | lru             | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      | 23272853     |
| no-write-allocate | write-through    | lru             | 318197     | 197486      | 308743   | 9454      | 160016    | 37470      | 21012197     |
| write-allocate    | write-back       | fifo            | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      | 3694383      |
| write-allocate    | write-through    | fifo            | 318197     | 197486      | 313766   | 4431      | 170130    | 27356      | 23272853     |
| no-write-allocate | write-through    | fifo            | 318197     | 197486      | 308743   | 9454      | 160016    | 37470      | 21012197     |

swim.trace tests:
| write-miss policy | write-hit policy | eviction policy | total load | total store | load hit | load miss | store hit | store miss | total cycles |
| ----------------- | ---------------- | --------------- | ---------- | ----------- | -------- | --------- | --------- | ---------- | ------------ |
| write-allocate    | write-back       | lru             | 220668     | 82525       | 219148   | 1520      | 61677     | 20848      | 2539993      |
| write-allocate    | write-through    | lru             | 220668     | 82525       | 219148   | 1520      | 61677     | 20848      | 10730816     |
| no-write-allocate | write-through    | lru             | 220668     | 82525       | 217217   | 3451      | 55354     | 27171      | 8818268      |
| write-allocate    | write-back       | fifo            | 220668     | 82525       | 219148   | 1520      | 61677     | 20848      | 2539993      |
| write-allocate    | write-through    | fifo            | 220668     | 82525       | 219148   | 1520      | 61677     | 20848      | 10730816     |
| no-write-allocate | write-through    | fifo            | 220668     | 82525       | 217217   | 3451      | 55354     | 27171      | 8818268      |