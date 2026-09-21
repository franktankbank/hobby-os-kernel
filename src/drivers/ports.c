#include "../types/int.h"

// /**
//  * Read a byte from the specified port
//  */
// unsigned char port_byte_in(u16 port) {
//     unsigned char result;
//     /* Inline assembler syntax
//      * !! Notice how the source and destination registers are switched from NASM !!
//      *
//      * '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
//      * '"d" (port)': map the C variable '(port)' into e'd'x register
//      *
//      * Inputs and outputs are separated by colons
//      */
//     asm("in %%dx, %%al" : "=a" (result) : "d" (port));
//     return result;
// }

// void port_byte_out(u16 port, u8 data) {
//     /* Notice how here both registers are mapped to C variables and
//      * nothing is returned, thus, no equals '=' in the asm syntax
//      * However we see a comma since there are two variables in the input area
//      * and none in the 'return' area
//      */
//     asm("out %%al, %%dx" : : "a" (data), "d" (port));
// }

// unsigned short port_word_in(u16 port) {
//     unsigned short result;
//     asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
//     return result;
// }

// void port_word_out(u16 port, u16 data) {
//     asm("out %%ax, %%dx" : : "a" (data), "d" (port));
// }

void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
