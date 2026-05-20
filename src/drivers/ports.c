#include <stdint.h>

// /**
//  * Read a byte from the specified port
//  */
// unsigned char port_byte_in(uint16_t port) {
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

// void port_byte_out(uint16_t port, uint8_t data) {
//     /* Notice how here both registers are mapped to C variables and
//      * nothing is returned, thus, no equals '=' in the asm syntax
//      * However we see a comma since there are two variables in the input area
//      * and none in the 'return' area
//      */
//     asm("out %%al, %%dx" : : "a" (data), "d" (port));
// }

// unsigned short port_word_in(uint16_t port) {
//     unsigned short result;
//     asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
//     return result;
// }

// void port_word_out(uint16_t port, uint16_t data) {
//     asm("out %%ax, %%dx" : : "a" (data), "d" (port));
// }

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
