#include "pic.h"
#include "../../drivers/ports.h"

void pic_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}
