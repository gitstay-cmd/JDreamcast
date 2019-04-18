#ifndef ICACHE_H
#define ICACHE_H

#include <stdint.h>

typedef struct s_icache {
    struct{
        uint32_t tag: 19;
        uint8_t v: 1;
    } ic_addr[256];
    uint32_t ic_data[256][8];
} icache;

void write_to_icache(icache *cache, uint32_t addr, uint32_t data);
uint32_t read_from_icache(icache *cache, uint32_t addr);
#endif

