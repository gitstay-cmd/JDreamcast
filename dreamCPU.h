#ifndef dreamCPU_H
#define dreamCPU_H

#include<stdint.h>
#include "mmu.h"

typedef struct S_CPU {
    uint32_t GR[16];
    uint32_t Banked_GR[8];
    // System registers
    struct {
        uint32_t MACH;      // Multiply-and-accumulate register high
        uint32_t MACL;      // Multiply-and-accumulate register low
        uint32_t PR;        // Procedure register
        uint32_t PC;        // Program counter
        uint32_t FPUL;      // Floating point communication register
        struct{    
            union {
                struct {
                    uint32_t RM: 2;
                    uint32_t FlagInEx: 1;
                    uint32_t FlagUnd: 1;
                    uint32_t FlagOv: 1;
                    uint32_t FlagDiv: 1;
                    uint32_t FlagInv: 1;
                    uint32_t EnInEx: 1;
                    uint32_t EnUnd: 1;
                    uint32_t EnOv: 1;
                    uint32_t EnDiv: 1;
                    uint32_t EnInv: 1;
                    uint32_t CauInEx: 1;
                    uint32_t CauUnd: 1;
                    uint32_t CauOv: 1;
                    uint32_t CauDiv: 1;
                    uint32_t CauInv: 1;
                    uint32_t CauFPUE: 1;
                    uint32_t DN: 1;
                    uint32_t PR: 1;
                    uint32_t SZ: 1;
                    uint32_t FR: 1;
                    uint32_t RES: 10;
                };
                uint32_t data;
            };
        } FPSCR;          // Floating-point status/control register
    };
    
    // Control Registers
    struct {
        struct{
            union {
                struct {
                    uint32_t T: 1;      // [0]      Carry/borrow bit
                    uint32_t S: 1;      // [1]      Specifies saturation operation for MAC
                    uint32_t RES1: 2;   // [2, 3]   Reserved
                    uint32_t IMASK: 4;  // [4, 7]   Interrupt mask level
                    uint32_t Q: 1;      // [8]      State for divide step
                    uint32_t M: 1;      // [9]      State for divide step
                    uint32_t RES2: 5;   // [10, 14] Reserved
                    uint32_t FD: 1;     // [15]     FU disable bit (cleared to 0 by a reset)
                    uint32_t RES3: 12;  // [16, 27] Reserved
                    uint32_t BL: 1;     // [28]     Exception/interrupt block bit( 1 by a reset)
                    uint32_t RB: 1;     // [29]     General register bank specifier in priviledged mode
                    uint32_t MD: 1;     // [30]     Processor Mode
                    uint32_t RES4: 1;   // [31]     Reserved
                };
                uint32_t data;
            };
        } SR;
        uint32_t SSR;   // Saved status register
        uint32_t SPC;   // Saved program counter
        uint32_t GBR;   // Global base register
        uint32_t VBR;   // Vector base register
        uint32_t SGR;   // Saved general register
        uint32_t DBR;   // Debug base register
    }; 

    // Floating-point registers
    union {
        float FP_R[16];
        double DR[8];
        struct {
            double data0;
            double data1;
        } FV[4];
    };

    union {
        float XFP_R[16];
        double DXFP_R[8];
        float XMATRIX[4][4];
    };
    
    uint32_t exc_code;

    MMU *mmu;
    uint8_t sleep: 1;
} CPU;

void exception(CPU *cpu);
#endif

