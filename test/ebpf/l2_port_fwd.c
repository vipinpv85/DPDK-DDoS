/* log file in ```cat /sys/kernel/debug/tracing/trace``` */

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>

#include "bpf_helpers.h"
#include "bpf_endian.h"

#include <stdint.h>
#include <stdio.h>

#define SEC(NAME) __attribute__((section(NAME), used))

#define bpf_printk(fmt, ...)                                    \
        ({                                                              \
                 char ____fmt[] = fmt;                                   \
                 bpf_trace_printk(____fmt, sizeof(____fmt),              \
                 ##__VA_ARGS__);                        \
        })

struct vlan_hdr {
        __be16  h_vlan_TCI;
        __be16  h_vlan_encapsulated_proto;
};

struct bpf_map_def SEC("maps") tx_port = {
        .type = BPF_MAP_TYPE_ARRAY,
        .key_size = sizeof(int),
        .value_size = sizeof(unsigned int),
        .max_entries = 32,
};

struct bpf_map_def SEC("maps") count_port = {
        .type = BPF_MAP_TYPE_ARRAY,
        .key_size = sizeof(int),
        .value_size = sizeof(unsigned long int) * 3, /* RX, DROP, TX */
        .max_entries = 32,
};

SEC("prog")
int balancer_ingress(struct xdp_md *ctx)
{
        __u32 input_port = ctx->ingress_ifindex;
        __u32 *out_port = bpf_map_lookup_elem(&tx_port, &input_port);

        void *data_end = (void *)(long)ctx->data_end;
        void *data = (void *)(long)ctx->data;
        struct ethhdr *eth_ptr;
        struct vlan_hdr *vlan_ptr;

        if (!out_port)
                return XDP_ABORTED;

        eth_ptr = (struct ethhdr *)(long)ctx->data;
        if ((eth_ptr + 1) > data_end)
                return XDP_ABORTED;

#if 0
        if (*out_port == 0) {
                //bpf_printk(" failed to find out port for in port (%llu)\n", input_port);
                return XDP_DROP;
        }
#endif

        if (eth_ptr->h_proto != bpf_htons(ETH_P_ARP)) {
                if ((eth_ptr->h_proto == bpf_htons(ETH_P_8021Q)) ||
                    (eth_ptr->h_proto == bpf_htons(ETH_P_8021AD))) {
                        vlan_ptr = (void *)(eth_ptr + 1);
                        if ((vlan_ptr + 1) > data_end)
                                return XDP_ABORTED;

                        //bpf_printk(" vlan_tci (0x%x) next_protocol (0x%x)\n", vlan_ptr->h_vlan_TCI, vlan_ptr->h_vlan_encapsulated_proto);
                        /* action: pop valn, lookup per port valn, or use vlan to find namespace and pop forward */
                        #pragma unroll
                        for (int i = 11; i >= 0; i--)
                        {
                                *((unsigned char *)eth_ptr + (i + 4)) = *((unsigned char *)eth_ptr + i);
                        }

                        if (bpf_xdp_adjust_head(ctx, (int)sizeof(struct vlan_hdr)))
                                return XDP_ABORTED;

                        eth_ptr = (struct ethhdr *)(long)ctx->data;
                        if ((eth_ptr + 1) > ctx->data_end)
                                return XDP_ABORTED;

                        eth_ptr = (unsigned char*) eth_ptr + 4;
                        if ((eth_ptr + 1) > data_end)
                                return XDP_ABORTED;

                }

                if ((eth_ptr->h_proto == bpf_htons(ETH_P_IP)) ||
                    (eth_ptr->h_proto == bpf_htons(ETH_P_IPV6))) {
                        //bpf_printk(" IPv4|IPv6: eth type %llx\n", eth_ptr->h_proto);
                        /* update the MAC address */
                        //bpf_printk(" before: 0x%llx\n", *((uint64_t *)eth_ptr->h_dest));
                        *((uint32_t *) (eth_ptr->h_dest + 2)) = (*out_port);
                        //bpf_printk(" after: 0x%llx\n", *((uint64_t *)eth_ptr->h_dest));
                }
                else {
                        bpf_printk(" DROP: eth type %llx\n", eth_ptr->h_proto);
                        return XDP_DROP;
                }
        }

        bpf_printk(" in %u out %u\n", input_port, *out_port);
        return bpf_redirect(*out_port, 0);
        return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
