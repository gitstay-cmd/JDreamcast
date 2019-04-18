#ifndef OCACHE_H
#define OCACHE_H

#include <stdint.h>

typedef struct s_ocache {
    struct{
        uint8_t u: 1;
        uint8_t v: 1;
        uint32_t tag: 19;
    } oc_addr[512];
    uint32_t oc_data[512][8];
} ocache;

void write_to_ocache(ocache *cache, uint32_t addr, uint32_t data);
uint32_t read_from_ocache(ocache *cache, uint32_t addr);
#endif

