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
 [ ] Primary configruation and Secodnary Processing
 [ ] Failure of secodnary, Primary pass through
 [ ] Flow calssification based on destination IP
 [ ] Inbound IP reassembly with ACL filtering
 [ ] service core based telemetry and data base update.
 [ ] eBPF inspired flow distribution to multiple workers


