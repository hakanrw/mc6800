#include <stdio.h>

#define CHIPS_IMPL
#include "chips/chips/mc6800.h"

int main(int argc, char** argv) {
    // setup 64 kBytes of memory
    uint8_t mem[1<<16] = { 0x86, 0x31, 0x97, 0xF0, 0x7C, 0x00, 0xF0, 0x7C, 0x00, 0xF0, 0x7A, 0x00, 0xF0, 0x9B, 0x01, 0x90, 0x01 };
    // initialize the CPU
    mc6800_t cpu;
    uint64_t pins = mc6800_init(&cpu);
    pins &= ~MC6800_RESET;
    printf("VMA: %d\n", pins & MC6800_VMA);
    printf("RESET: %d\n", pins & MC6800_RESET);
    printf("RW: %d\n", pins & MC6800_RW);
    printf("HALT: %d\n", pins & MC6800_HALT);
    printf("mem 10: %x\n", mem[0x10]);

    MC6800_SET_DATA(pins, mem[0]);
    while (1) {
        if (cpu.PC >= 17 && cpu.next_instr) break;
        printf("\n--------------------\n");

        printf("PC: %d\n", cpu.PC);
        pins = mc6800_tick(&cpu, pins);

        const uint16_t addr = MC6800_GET_ADDR(pins);
        printf("adr: %x\n", MC6800_GET_ADDR(pins));
        printf("data: %x\n", MC6800_GET_DATA(pins));
        printf("RW: %d\n", pins & MC6800_RW ? 1 : 0);
        printf("VMA: %d\n", pins & MC6800_VMA ? 1 : 0);

        // extract 16-bit address from pin mask
        // perform memory access
        if (pins & MC6800_VMA) {
            if (pins & MC6800_RW) {
                // a memory read
                MC6800_SET_DATA(pins, mem[addr]);
            }
            else {
                // a memory write
                mem[addr] = MC6800_GET_DATA(pins);
            }
        }

        printf("m,data: %x\n", MC6800_GET_DATA(pins));

        // run the CPU emulation for one tick
        printf("nexts: %d\n", cpu.next_instr);
    }

    printf("0xF0: %x\n", mem[0xF0]);
    printf("areg: %x\n", cpu.A);

    return 0;
}
