#include "serial.h"
#include "ports.h"

void serial_init(void) {
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // Divisor low byte (38400 baud)
    outb(COM1 + 1, 0x00); // Divisor high byte
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // Enable FIFO
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int serial_received() {
    return inb(COM1 + 5) & 1;
}

char read_serial() {
    while (serial_received() == 0);

    return inb(COM1);
}

int is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

void write_serial(char a) {
    while (is_transmit_empty() == 0);

    outb(COM1, a);
}
