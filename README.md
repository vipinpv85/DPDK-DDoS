# DPDK-DDoS
simplified DDoS prototype using DPDK application model.

## Motivation
to leverage DPDK libraries and PMD feature to build line rate processing for
- zero buffer copy
- reduce failover on faulty cases
- de-centralized telemetry
- in bound no Relational Data Base

## Design

## To Do
 - [x] Primary configruation and Secodnary
 - [ ] Failure of secodnary, Primary pass through
 - [ ] Flow calssification based on destination IP
 - [ ] Inbound IP reassembly with ACL filtering
 - [ ] service core based telemetry and data base update.
 - [ ] eBPF inspired flow distribution to multiple workers

## How to Build?

 - EBPF: `clang-9 -O2 -emit-llvm -I /usr/include/linux -c drop.c -o -| llc -march=bpf -mcpu=probe -filetype=obj -o test.o`

### dependency
 - DPDK
 - libnuma-dev

### version:
| software | release |
| -- | -- |
| DPDK | dpdk-stable-18.11.4 |

### Build and Run

#### DPDK 18.11.4
- Download DPDK from dpdk.org.
- Untar DPDK tar file.
- Execute the following commands
```
 cd <to unatar dpdk folder>
 make config T=x86_64-native-linuxapp-gcc O=x86_64-native-linuxapp-gcc
 export RTE_SDK=$PWD
 export RTE_TARGET=x86_64-native-linuxapp-gcc
 cd x86_64-native-linuxapp-gcc
 make -j 4
```
- Test the custom build by cross checking examples like helloworld & l2fwd.

### application
 - ```meson <build directory>```
 - ```ninja -C <build directory>```

