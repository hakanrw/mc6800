#include <stdio.h>

#define CHIPS_IMPL
#include "chips/chips/mc6800.h"

int main(int argc, char** argv) {
    // setup 64 kBytes of memory
    uint8_t mem[1<<16] = { };
    // initialize the CPU
    mc6800_t cpu;
    uint64_t pins = mc6800_init(&cpu);

    pins &= ~MC6800_RESET;
    printf("VMA: %d\n", pins & MC6800_VMA);
    printf("RESET: %d\n", pins & MC6800_RESET);
    printf("RW: %d\n", pins & MC6800_RW);
    printf("HALT: %d\n", pins & MC6800_HALT);
    printf("mem 10: %x\n", mem[0x10]);

    // Open the file in binary read mode
    FILE* file = fopen("obj/out", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the contents of the file into the memory array
    size_t bytes_read = fread(mem, 1, sizeof(mem), file);
    if (bytes_read != sizeof(mem) && !feof(file)) {
        perror("Error reading file");
        fclose(file);
        return 1;
    }

    printf("Read %zu bytes into ROM\n", bytes_read);

    MC6800_SET_DATA(pins, mem[0]);
    while (1) {
        if (cpu.PC >= bytes_read && cpu.next_instr) break;

        if (cpu.next_instr) printf("\n********************\n======== %02X ========\n", mem[cpu.PC]);
        else printf("\n--------------------\n");

        printf("PC: %d\n", cpu.PC);
        pins = mc6800_tick(&cpu, pins);
        const uint16_t addr = MC6800_GET_ADDR(pins);
        printf("addr: %x\n", MC6800_GET_ADDR(pins));
        printf("data: %x\n", MC6800_GET_DATA(pins));
        printf("RW: %d\n", pins & MC6800_RW ? 1 : 0);
        printf("VMA: %d\n", pins & MC6800_VMA ? 1 : 0);

        printf("A: %02X\n", cpu.A);
        printf("B: %02X\n", cpu.B);
        printf("IX: %04X\n", cpu.IX);
        printf("SP: %04X\n", cpu.SP);
        printf("H:%d I:%d N:%d Z:%d V:%d C:%d\n", cpu.P&MC6800_HF?1:0, cpu.P&MC6800_IF?1:0, cpu.P&MC6800_NF?1:0, cpu.P&MC6800_ZF?1:0, cpu.P&MC6800_VF?1:0, cpu.P&MC6800_CF?1:0);

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

    }

    puts("");
    printf("0x2000: %d\n", mem[0x2000]);
    printf("0x2001: %d\n", mem[0x2001]);
    printf("0x2002: %d\n", mem[0x2002]);
    printf("0x2003: %d\n", mem[0x2003]);
    printf("0x2004: %d\n", mem[0x2004]);
    printf("0x2005: %d\n", mem[0x2005]);
    printf("0x2006: %d\n", mem[0x2006]);
    printf("0x2007: %d\n", mem[0x2007]);
    printf("0x2008: %d\n", mem[0x2008]);
    printf("0x2009: %d\n", mem[0x2009]);
    printf("0x200A: %d\n", mem[0x200A]);
    printf("0x200B: %d\n", mem[0x200B]);

    return 0;
}
