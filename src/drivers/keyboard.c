// #include "../cpu/isr.h"
// #include "../kernel/console.h"
// #include "ports.h"

// void print_letter(uint8_t scancode) {
//   switch (scancode) {
//   case 0x0:
//     Console.puts("ERROR");
//     break;
//   case 0x1:
//     Console.puts("ESC");
//     break;
//   case 0x2:
//     Console.puts("1");
//     break;
//   case 0x3:
//     Console.puts("2");
//     break;
//   case 0x4:
//     Console.puts("3");
//     break;
//   case 0x5:
//     Console.puts("4");
//     break;
//   case 0x6:
//     Console.puts("5");
//     break;
//   case 0x7:
//     Console.puts("6");
//     break;
//   case 0x8:
//     Console.puts("7");
//     break;
//   case 0x9:
//     Console.puts("8");
//     break;
//   case 0xA:
//     Console.puts("9");
//     break;
//   case 0xB:
//     Console.puts("0");
//     break;
//   case 0xC:
//     Console.puts("-");
//     break;
//   case 0xD:
//     Console.puts("=");
//     break;
//   case 0xE:
//     Console.puts("BACKSPACE");
//     break;
//   case 0xF:
//     Console.puts("TAB");
//     break;
//   case 0x10:
//     Console.puts("Q");
//     break;
//   case 0x11:
//     Console.puts("W");
//     break;
//   case 0x12:
//     Console.puts("E");
//     break;
//   case 0x13:
//     Console.puts("R");
//     break;
//   case 0x14:
//     Console.puts("T");
//     break;
//   case 0x15:
//     Console.puts("Y");
//     break;
//   case 0x16:
//     Console.puts("U");
//     break;
//   case 0x17:
//     Console.puts("I");
//     break;
//   case 0x18:
//     Console.puts("O");
//     break;
//   case 0x19:
//     Console.puts("P");
//     break;
//   case 0x1A:
//     Console.puts("[");
//     break;
//   case 0x1B:
//     Console.puts("]");
//     break;
//   case 0x1C:
//     Console.puts("ENTER");
//     break;
//   case 0x1D:
//     Console.puts("LCTRL");
//     break;
//   case 0x1E:
//     Console.puts("A");
//     break;
//   case 0x1F:
//     Console.puts("S");
//     break;
//   case 0x20:
//     Console.puts("D");
//     break;
//   case 0x21:
//     Console.puts("F");
//     break;
//   case 0x22:
//     Console.puts("G");
//     break;
//   case 0x23:
//     Console.puts("H");
//     break;
//   case 0x24:
//     Console.puts("J");
//     break;
//   case 0x25:
//     Console.puts("K");
//     break;
//   case 0x26:
//     Console.puts("L");
//     break;
//   case 0x27:
//     Console.puts(";");
//     break;
//   case 0x28:
//     Console.puts("\'");
//     break;
//   case 0x29:
//     Console.puts("`");
//     break;
//   case 0x2A:
//     Console.puts("LSHIFT");
//     break;
//   case 0x2B:
//     Console.puts("\\");
//     break;
//   case 0x2C:
//     Console.puts("Z");
//     break;
//   case 0x2D:
//     Console.puts("X");
//     break;
//   case 0x2E:
//     Console.puts("C");
//     break;
//   case 0x2F:
//     Console.puts("V");
//     break;
//   case 0x30:
//     Console.puts("B");
//     break;
//   case 0x31:
//     Console.puts("N");
//     break;
//   case 0x32:
//     Console.puts("M");
//     break;
//   case 0x33:
//     Console.puts(",");
//     break;
//   case 0x34:
//     Console.puts(".");
//     break;
//   case 0x35:
//     Console.puts("/");
//     break;
//   case 0x36:
//     Console.puts("RSHIFT");
//     break;
//   case 0x37:
//     Console.puts("(KEYPAD) *");
//     break;
//   case 0x38:
//     Console.puts("LALT");
//     break;
//   case 0x39:
//     Console.puts("SPACE");
//     break;
//   case 0x3A:
//     Console.puts("CAPSLOCK");
//     break;
//   case 0x3B:
//     Console.puts("F1");
//     break;
//   case 0x3C:
//     Console.puts("F2");
//     break;
//   case 0x3D:
//     Console.puts("F3");
//     break;
//   case 0x3E:
//     Console.puts("F4");
//     break;
//   case 0x3F:
//     Console.puts("F5");
//     break;
//   case 0x40:
//     Console.puts("F6");
//     break;
//   case 0x41:
//     Console.puts("F7");
//     break;
//   case 0x42:
//     Console.puts("F8");
//     break;
//   case 0x43:
//     Console.puts("F9");
//     break;
//   case 0x44:
//     Console.puts("F10");
//     break;
//   case 0x45:
//     Console.puts("NUMLOCK");
//     break;
//   case 0x46:
//     Console.puts("SCROLLLOCK");
//     break;
//   case 0x47:
//     Console.puts("(KEYPAD) 7");
//     break;
//   case 0x48:
//     Console.puts("(KEYPAD) 8");
//     break;
//   case 0x49:
//     Console.puts("(KEYPAD) 9");
//     break;
//   case 0x4A:
//     Console.puts("(KEYPAD) -");
//     break;
//   case 0x4B:
//     Console.puts("(KEYPAD) 4");
//     break;
//   case 0x4C:
//     Console.puts("(KEYPAD) 5");
//     break;
//   case 0x4D:
//     Console.puts("(KEYPAD) 6");
//     break;
//   case 0x4E:
//     Console.puts("(KEYPAD) +");
//     break;
//   case 0x4F:
//     Console.puts("(KEYPAD) 1");
//     break;
//   case 0x50:
//     Console.puts("(KEYPAD) 2");
//     break;
//   case 0x51:
//     Console.puts("(KEYPAD) 3");
//     break;
//   case 0x52:
//     Console.puts("(KEYPAD) 0");
//     break;
//   case 0x53:
//     Console.puts("(KEYPAD) .");
//     break;
//   case 0x57:
//     Console.puts("F11");
//     break;
//   case 0x58:
//     Console.puts("F12");
//     break;
//   default:
//     if (scancode <= 0x7f) {
//       Console.puts("Unknown key down");
//     } else if (scancode <= 0x58 + 0x80) {
//       Console.puts("key up ");
//       print_letter(scancode - 0x80);
//     } else {
//       Console.puts("Unknown key up");
//     }
//     break;
//   }
// }

// static void keyboard_callback(registers_t *regs) {
//   uint8_t scancode = port_byte_in(0x60);
//   print_letter(scancode);
//   Console.putc((unsigned char)'\n');
// }

// void init_keyboard() { register_interrupt_handler(IRQ1, keyboard_callback); }
