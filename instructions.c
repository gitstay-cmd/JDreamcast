#include <stdint.h>
#include "dreamCPU.h"

#define MASK_URC    0xFC00  
#define URC_OFFSET  10

void ldtlb(CPU *cpu){
    uint32_t urc = cpu->mmu->URC;

}
