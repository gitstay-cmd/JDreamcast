#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include "icache.h"
#include "ocache.h"

typedef struct {
    icache incache;
    ocache opcache;
    uint32_t QACR0;
    uint32_t QACR1;
    union {
        struct{
            uint8_t OCE: 1;
            uint8_t WT: 1;
            uint8_t CB: 1;
            uint8_t OCI: 1;
            uint8_t res1: 1;
            uint8_t ORA: 1;
            uint8_t res2: 1;
            uint8_t OIX: 1;
            uint8_t ICE: 1;
            uint8_t res3: 2;
            uint8_t ICI: 1;
            uint8_t res4: 3;
            uint8_t IIX: 1;
            uint16_t res5;
        };
        uint32_t CCR;
    };
}cache;

#endif
