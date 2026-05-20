#define COM1 0x3F8

void serial_init(void);

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char a);
