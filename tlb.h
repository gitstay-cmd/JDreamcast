#ifndef TLB_H
#define TLB_H

typedef struct S_TLB_E{
    uint8_t v: 1;
    uint8_t asid;
    uint32_t vpn;
    uint32_t ppn;
    uint8_t sz: 2;
    uint8_t sh: 1;
    uint8_t c: 1;
    uint8_t pr: 2;
    uint8_t d: 1;
    uint8_t wt: 1;
} TLB_E;


#endif
