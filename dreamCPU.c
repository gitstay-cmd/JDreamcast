#include "dreamCPU.h"

#include <stdio.h>

#define EX_HANDLER 0xA0000000

#define TLB_MISS    0x40
#define TLB_MISS_OFFSET 0x0400

#define TLB_MISS_WRITE  0x60
void reset(CPU *cpu){
    cpu->PC = EX_HANDLER;
}

/*
 * Probably a gross simplification of dealing with basic interrupts
 * */
void interrupt(CPU *cpu){
    //cpu->SPC = cpu->PC;
    //cpu->SSR = cpu->SR;
    //cpu->SGR = cpu->GR[0xF];  
}

void exception(CPU *cpu){
    switch(cpu->exc_code){
        case  TLB_MISS:
          cpu->SPC = cpu->PC;
          cpu->SSR = cpu->SR.data;
          cpu->SR.MD = 1;
          cpu->SR.BL = 1;
          cpu->SR.RB = 1;
          cpu->PC = cpu->VBR + TLB_MISS_OFFSET;
          break;
        case  TLB_MISS_WRITE:
          cpu->SPC = cpu->PC;
          cpu->SSR = cpu->SR.data;
          cpu->SR.MD = 1;
          cpu->SR.BL = 1;
          cpu->SR.RB = 1;
          cpu->PC = cpu->VBR + TLB_MISS_OFFSET;
          break;
        default:
          printf("exception\n");
          break;
    }
}

void process_instruction(CPU *cpu){
    
}

