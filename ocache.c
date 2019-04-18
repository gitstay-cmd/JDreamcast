#include "ocache.h"
#include "dreamCPU.h"


#define MASK_ENTRY 0xFF0
#define ENTRY_OFFSET 5

#define MASK_LONGWORD 0x1C
#define LONGWORD_OFFSET 2

#define TAG_OFFSET 10

#define MASK_A 0x8
#define A_OFFSET 3

#define MASK_U 0x2
#define U_OFFSET  1

void write_to_ocache(ocache *cache, uint32_t addr, uint32_t data){
    if((addr & 0xFF000000) == 0xF0000000){
        //Writes to the address
        uint8_t A = (addr & MASK_A) >> A_OFFSET;
        uint32_t entry = (addr & MASK_ENTRY) >> ENTRY_OFFSET;
        uint32_t longword = (addr & MASK_LONGWORD) >> LONGWORD_OFFSET;
        uint32_t tag = (data) >> TAG_OFFSET;
        uint8_t v = data & 0x01;
        uint8_t u = (data & MASK_U) >> U_OFFSET;
        if(A){
            //Implement OC entry invalidation
        }else {
            cache->oc_addr[entry].tag = tag;
            cache->oc_addr[entry].v = v;
            cache->oc_addr[entry].u = u;
        }
    } else {
        //Writes to the address
        uint32_t entry = (addr & MASK_ENTRY) >> ENTRY_OFFSET;
        uint32_t longword = (addr & MASK_LONGWORD) >> LONGWORD_OFFSET;
        cache->oc_data[entry][longword] = data;
    }
}

uint32_t read_from_ocache(ocache *cache, uint32_t addr){
   if((addr & 0xFF000000) == 0xF0000000){
        //Writes to the address
        uint32_t entry = (addr & MASK_ENTRY) >> ENTRY_OFFSET;
        uint32_t tag = cache->oc_addr[entry].tag;
        uint8_t v = cache->oc_addr[entry].v;
        uint8_t u = cache->oc_addr[entry].u;
        uint32_t data = (tag << 10) |(u << U_OFFSET) | v;
        return data;
    } else {
        //Writes to the address
        uint32_t entry = (addr & MASK_ENTRY) >> ENTRY_OFFSET;
        uint32_t longword = (addr & MASK_LONGWORD) >> LONGWORD_OFFSET;
        return cache->oc_data[entry][longword];
    }

}
