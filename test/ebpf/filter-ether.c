#include <linux/bpf.h>
#include <linux/if_ether.h>

#include "bpf_helpers.h"
#include "bpf_endian.h"

#include <stdint.h>
#include <stdio.h>

#define SEC(NAME) __attribute__((section(NAME), used))

#define ETH_P_BE_MYHEADER 0xADDE
#define ETH_MYHEADER_SIZE (0x04)

struct myheader_hdr {
        __be16  h_conn_id;
        __be16  h_dest_id;
};

struct bpf_map_def SEC("maps") map_count = {
        .type = BPF_MAP_TYPE_ARRAY,
        .key_size = sizeof(int),
        .value_size = sizeof(unsigned long int) * 3, /* RX, DROP, TX */
        .max_entries = 32,
};

SEC("filter_ingress_ethtype")
int ext_intf_ingress(struct xdp_md *ctx)
{
        __u32 input_port = ctx->ingress_ifindex;
        __u32 key = 0;
        __u64 *count_rx;
        __u64 *count_tx;
        __u64 *count_drop;

        count_rx = bpf_map_lookup_elem(&map_count, &key);
        if (!count_rx)
                return XDP_ABORTED;
        __sync_fetch_and_add(count_rx, 1);

        void *data_end = (void *)(long)ctx->data_end;
        void *data = (void *)(long)ctx->data;

        struct ethhdr *eth_ptr;
        struct myheader_hdr *custom_ptr;

        eth_ptr = (struct ethhdr *)(long)data;
        if ((eth_ptr + 1) > data_end)
                return XDP_ABORTED;

        if (eth_ptr->h_proto == ETH_P_BE_MYHEADER) {
                custom_ptr = (void *)(eth_ptr + 1);
                if ((custom_ptr + 1) > data_end)
                        return XDP_ABORTED;

                #pragma unroll
                for (int i = 11; i >= 0; i--)
                {
                        *((unsigned char *)eth_ptr + (i + 4)) = *((unsigned char *)eth_ptr + i);
                }
                }

                if (bpf_xdp_adjust_head(ctx, (int)sizeof(struct myheader_hdr)))
                        return XDP_ABORTED;

                data_end = (void *)(long)ctx->data_end;
                data = (void *)(long)ctx->data;

                if ((long)(data + 16) > (long)data_end)
                        return XDP_ABORTED;

                key = 2;
                count_tx = bpf_map_lookup_elem(&map_count, &key);
                if (!count_tx)
                        return XDP_ABORTED;

                __sync_fetch_and_add(count_tx, 1);
                return XDP_PASS;
        }

        key = 1;
        count_drop = bpf_map_lookup_elem(&map_count, &key);
        if (!count_drop)
                return XDP_ABORTED;

        __sync_fetch_and_add(count_drop, 1);
        return XDP_DROP;
}

char _license[] SEC("license") = "GPL";
