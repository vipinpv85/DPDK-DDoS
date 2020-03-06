#include <linux/bpf.h>

#include <stdint.h>

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("prog")
int xdp_select(struct xdp_md *ctx)
{
        return XDP_PASS;
}
