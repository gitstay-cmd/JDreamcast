#include  "mmu.h"
#include  "dreamCPU.h"
#include  <stdio.h>

enum page_size{
    KBYTE = 0x00,
    FRKBYTE,
    SFRKBYTE,
    MBYTE,
};

#define MASK_P  0xF0000000
#define P_OFFSET 0x7

#define P0  0x0
#define P1  0x8
#define P2  0xA
#define P3  0xC
#define P4  0xE

#define   MASK_SQ   0x20
#define   SQ_OFFSET 0x5

#define   MASK_LW   0x1C
#define   LW_OFFSET 0x3

#define   MASK_VPN1 0xFFFFFC00
#define   VPN1_OFFSET 0xA

#define   MASK_VPN2 0xFFFFF000
#define   VPN2_OFFSET 0xC

#define   MASK_VPN3 0xFFFF0000
#define   VPN3_OFFSET 0x10

#define   MASK_VPN4 0xFFF00000
#define   VPN4_OFFSET 0x14

uint8_t user_mode(CPU *cpu){
    return cpu->SR.MD;
}

uint8_t get_size(CPU *cpu){
    return (cpu->mmu->SZ0 | (cpu->mmu->SZ1 << 1));
}

uint8_t search_utlb(CPU *cpu, uint32_t *page, uint32_t vpn){
    for(int i = 0; i < 64;i++){
        if(cpu->mmu->utlb[i].vpn == vpn){
            if(cpu->mmu->utlb[i].v){
                *page = cpu->mmu->utlb[i].ppn;
                return 1;
            }
        }
    }
    return 0;
}

uint8_t search_itlb(CPU *cpu, uint32_t *page, uint32_t vpn){
    for(int i = 0; i < 64;i++){
        if(cpu->mmu->utlb[i].vpn == vpn){
            if(cpu->mmu->utlb[i].v){
                *page = cpu->mmu->utlb[i].ppn;
                return 1;
            }
        }
    }
    return 0;
}


uint32_t get_page (CPU *cpu, uint32_t addr, uint8_t inst){
    uint8_t p = (addr & MASK_P) >> P_OFFSET;
    uint32_t vpn;
    switch(get_size(cpu)){
        case KBYTE:
          vpn = (addr & MASK_VPN1) >> VPN1_OFFSET;
          break;
        case FRKBYTE:
          vpn = (addr & MASK_VPN2) >> VPN2_OFFSET;
          break;
        case SFRKBYTE:
          vpn = (addr & MASK_VPN3) >> VPN3_OFFSET;
          break;
        case MBYTE:
          vpn = (addr & MASK_VPN4) >> VPN4_OFFSET;
          break;
        default:
          break;
    }
    uint32_t *page;
    if(p < P1){
        uint8_t res;
        if(inst){
            res = search_utlb(cpu, page, vpn);
        }else {
            res = search_utlb(cpu, page, vpn);
        }
        if(!res){
            exception(cpu);
        }
    } else if(p < P2) {
        // No address translation
    } else if(p < P3){
        // No address translation
    } else if(p < P4) {
        uint8_t res;
        if(inst){
            res = search_utlb(cpu, page, vpn);
        }else {
            res = search_utlb(cpu, page, vpn);
        }
        if(!res){
            exception(cpu);
        }
    } else {
        // No address translation
    }
    return *page;
}

void store_queue_write(MMU *mmu, uint32_t addr, uint32_t data){
    uint8_t sq = (addr & MASK_SQ) >> SQ_OFFSET;
    uint8_t lw = (addr & MASK_LW) >> LW_OFFSET;
    if(sq){
        mmu->store_queue1->queue[lw] = data;
    } else {
        mmu->store_queue0->queue[lw] = data;
    }

}

uint32_t store_queue_read(MMU *mmu, uint32_t addr){
    uint8_t sq = (addr & MASK_SQ) >> SQ_OFFSET;
    uint8_t lw = (addr & MASK_LW) >> LW_OFFSET;
    if(sq){
        return mmu->store_queue1->queue[lw];
    } else {
        return mmu->store_queue0->queue[lw];
    }
}

void write_byte(struct S_CPU *cpu, uint32_t addr, uint8_t data){
    
    if((addr % 8) != 0){
        exception(cpu);
        return;
    }
    //uint32_t raddr = get_address(addr);
    switch(addr & 0xFF000000){
        case 0x1F000000:
        case 0xFF000000:
            
        break;
    }
    //cpu->mmu->mem[raddr] = data;
}

void write_halfword(struct S_CPU *cpu, uint32_t addr, uint16_t data){
    
    write_byte(cpu, addr, (uint8_t)data);
    write_byte(cpu, addr+1, (uint8_t)(data >> 8));
}

void write_word(struct S_CPU *cpu, uint32_t addr, uint32_t data){
    switch(addr & 0xFF000000){
        case  0x80000000:
        case  0x90000000:
            if(user_mode(cpu)){
                exception(cpu);
            }
        //priveledged mode only
        break;
        case  0xA0000000:
        case  0xB0000000:
            if(user_mode(cpu)){
                exception(cpu);
            }
        //priveledged mode and no cache
        break;
        case  0xC0000000:
        case  0xD0000000:
            if(user_mode(cpu)){
                exception(cpu);
            }
        //priveledged mode and address translation
        case  0xE0000000:
        //Start of p4
        break;
        case  0xFF000000:
            switch(addr & 0xFF){
                case 0x00:
                    cpu->mmu->PTEH = data;
                break;
                case 0x04:
                    cpu->mmu->PTEL = data;
                break;
                case 0x08:
                    cpu->mmu->TTB = data;
                break;
                case 0x0C:
                    cpu->mmu->TEA = data;
                break;
                case 0x10:
                    cpu->mmu->MMUCR = data;
                break;
                default:
                break;
            }
            uint16_t tail = addr & 0xFFFF; 
            if(tail > 0x1000 && tail < 0x103C){
                store_queue_write(cpu->mmu, addr, data);
            }
        break;
        default:
            cpu->mmu->mem[addr] = data;
        break;
    }
}

uint8_t read_byte(struct S_CPU *cpu, uint32_t addr, uint8_t inst){
    return cpu->mmu->mem[addr];
}

uint16_t read_halfword(struct S_CPU *cpu, uint32_t addr, uint8_t inst){
    uint16_t data = read_byte(cpu, addr, inst);
    data = (data << 8) | read_byte(cpu,addr+1, inst);
    return data;
}

uint32_t read_word(struct S_CPU *cpu, uint32_t addr, uint8_t inst){
    
    switch(addr & 0xFF000000){
        case  0x80000000:
        case  0x90000000:
            if(user_mode(cpu)){
                exception(cpu);
            }
        //priveledged mode only
        break;
        case  0xA0000000:
        case  0xB0000000:
            if(user_mode(cpu)){
                exception(cpu);
            }
        //priveledged mode and no cache
        break;
        case  0xC0000000:
        case  0xD0000000:
            if(user_mode(cpu)){
                exception(cpu);
            }
        //priveledged mode and address translation
        case  0xE0000000:
        //Start of p4
            if((addr & 0xFF) == 0xFC){
                printf("False\n");
                return 0xFFFFFFFF;
            }
            return store_queue_read(cpu->mmu, addr);

        break;
        case  0xFF000000:
            switch(addr & 0xFF){
                case 0x00:
                    return cpu->mmu->PTEH;
                break;
                case 0x04:
                    return cpu->mmu->PTEL;
                break;
                case 0x08:
                    return cpu->mmu->TTB;
                break;
                case 0x0C:
                    return cpu->mmu->TEA;
                break;
                case 0x10:
                    return cpu->mmu->MMUCR;
                break;
                default:
                break;
            }
            uint16_t tail = addr & 0xFFFF; 
            if(tail > 0x1000 && tail < 0x103C){
                return store_queue_read(cpu->mmu, addr);
            }
        break;
        default:
            return cpu->mmu->mem[addr];
        break;
    }


}
