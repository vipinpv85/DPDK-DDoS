#include <linux/bpf.h>
#include "bpf_helpers.h"
#include "bpf_endian.h"

#include <stdint.h>


#define SEC(NAME) __attribute__((section(NAME), used))

struct bpf_map_def SEC("maps") tx_port = {
        .type = BPF_MAP_TYPE_ARRAY,
        .key_size = sizeof(int),
        .value_size = sizeof(unsigned int),
        .max_entries = 256,
};

SEC("prog")
int forwarder(struct xdp_md *ctx)
{
        __u32 input_port = ctx->ingress_ifindex;
        __u32 *out_port = bpf_map_lookup_elem(&tx_port, &input_port);
        if (!out_port)
                return XDP_DROP;

        if (*out_port == 0)
                return XDP_DROP;

        return bpf_redirect(*out_port, 0);
}
