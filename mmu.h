#ifndef MMU_H

#include <stdint.h>
#include "cache.h"
#include "tlb.h"
#include "store_queue.h"

#define MAX_MEM 0x100000000
#define MMU_H

/*
 *    External memory space
 *            Area 0
 *          0x0000 0000
 *            Area 1
 *          0x0400 0000
 *            Area 2
 *          0x0800 0000
 *            Area 3
 *          0x0C00 0000
 *            Area 4
 *          0x1000 0000
 *            Area 5
 *          0x1400 0000
 *            Area 6
 *          0x1800 0000
 *            Area 7 ( reserved area )
 *          0x1C00 0000
 * */

struct S_CPU;

typedef struct S_MMU {
    union{    /* 0xFF00 0000 - 0x1F00 0000*/
        struct {
            uint8_t ASID;   /* Address space identifier */
            uint8_t resv: 2; /* Reserved */
            uint32_t VPN: 22; /* Virtual page number */
        };
        uint32_t PTEH;  /* Page table entry high register*/
    }; 
    union{    /* 0xFF00 0004 - 0x1F00 0004*/
        struct {
            uint8_t WT: 1; /* Write-through bit */
            uint8_t SH: 1; /* Share status bit */
            uint8_t D: 1; /* Dirty bit */
            uint8_t C: 1; /* Cacheability bit */
            uint8_t SZ0: 1; /* Page size bit */
            uint8_t PR: 2; /* Protection key data */
            uint8_t SZ1: 1; /* Page size bit */
            uint8_t V: 1; /* Validity bit */
            uint32_t PPN: 19; /* Physical page number */
            uint8_t RES: 4; /* Bits reserved */
        };
        uint32_t PTEL;  /* Page table entry low register*/
    };
    uint32_t TTB;   /* Translation table base register*/
    uint32_t TEA;   /* Translation table address register*/
    union {
        struct{
            uint8_t AT: 1; /* Address translation bit */
            uint8_t RES1: 1; /* Reserved 1 */
            uint8_t TI: 1; /* TLB invalidate */
            uint8_t RES2: 5; /* Reserved 2 */
            uint8_t SV: 1; /* Single virtual mode bit */
            uint8_t SQMD: 1; /* Store queue mode bit */
            uint8_t URC: 6; /* UTLB replace counter*/
            uint8_t RES3: 2; /* Reserved 3 */
            uint8_t URB: 6; /* UTLB replace boundary */
            uint8_t RES4: 2; /* Reserved 4 */
            uint8_t LRUI: 6; /* Least recently used ITLB */
        };
        uint32_t MMUCR;  /* MMU control register */
    };
    s_queue *store_queue0;
    s_queue *store_queue1;
    TLB_E utlb[64];
    TLB_E itlb[4];
    uint8_t mem[MAX_MEM];
} MMU;

void init_mmu(MMU *mmu);

void write_byte(struct S_CPU *cpu, uint32_t addr, uint8_t data);
void write_halfword(struct S_CPU *cpu, uint32_t addr, uint16_t data);
void write_word(struct S_CPU *cpu, uint32_t addr, uint32_t data);

uint8_t read_byte(struct S_CPU *cpu, uint32_t addr, uint8_t inst);
uint16_t read_halfword(struct S_CPU *cpu, uint32_t addr, uint8_t inst);
uint32_t read_word(struct S_CPU *cpu, uint32_t addr, uint8_t inst); 

#endif
