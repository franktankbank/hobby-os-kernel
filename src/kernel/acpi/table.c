#include "table.h"
#include "../globals.h"
#include "../pmm.h"
#include "../console.h"
#include "../util.h"

void *find_acpi_table(void *RootSDT, char signature[4]) {
    XSDT *xsdt = (XSDT *) RootSDT;
    int entries = (xsdt->h.Length - sizeof(xsdt->h)) / 8;

    for (int i = 0; i < entries; i++) {
        ACPI_SDT_Header *h = (ACPI_SDT_Header *) make_hhdm(xsdt->PointerToOtherSDT[i]);

        if (!strncmp(h->Signature, signature, 4))
            return (void *) h;
    }

    // Table not found
    return NULL;
}

bool verify_checksum(ACPI_SDT_Header *tableHeader) {
    unsigned char sum = 0;

    for (u32 i = 0; i < tableHeader->Length; i++) {
        sum += ((char *) tableHeader)[i];
    }

    return sum == 0;
}

void write_ioapic_register(volatile u32 *apic_base, u8 offset, u32 val) {
    /* tell IOREGSEL where we want to write to */
    apic_base[0] = offset;
    /* write the value to IOWIN */
    apic_base[4] = val;
}

u32 read_ioapic_register(volatile u32 *apic_base, u8 offset) {
    /* tell IOREGSEL where we want to read from */
    apic_base[0] = offset;
    /* return the data from IOWIN */
    return apic_base[4];
}

void ioapic_set_irq(u8 irq, u64 apic_id, u8 vector) {
    const u8 low_index = 0x10 + irq * 2;
    const u8 high_index = 0x10 + irq * 2 + 1;

    RedirectionEntry redir_entry;

    // Set APIC ID in high register
    redir_entry.upperDword =
        read_ioapic_register(limine_parsed_data.ioapic_virt, high_index);

    SerialConsole.printf("read register\n");

    redir_entry.destination = apic_id;

    write_ioapic_register(
        limine_parsed_data.ioapic_virt,
        high_index,
        redir_entry.upperDword
    );

    // Configure low register: unmask, fixed delivery, set vector
    redir_entry.lowerDword =
        read_ioapic_register(limine_parsed_data.ioapic_virt, low_index);

    redir_entry.mask = 0;
    redir_entry.destMode = 0;
    redir_entry.delvMode = 0;
    redir_entry.pinPolarity = 0;
    redir_entry.triggerMode = 0;
    redir_entry.vector = vector;

    write_ioapic_register(
        limine_parsed_data.ioapic_virt,
        low_index,
        redir_entry.lowerDword
    );
}

// Given: madt pointer, target_irq (e.g. 11 for audio)
// Returns: GSI for that IRQ (identity-mapped if no override found)

u32 madt_get_gsi(MADT *madt, u8 target_irq) {
    u32 table_len = rd32((u8 *)madt + 4); // ACPI table header: Length at offset 4
    u8 *entry = (u8 *)madt + 44; // skip MADT header (44 bytes)
    u8 *end   = (u8 *)madt + table_len;

    SerialConsole.printf("table_len=%#x\n", table_len);

    SerialConsole.printf("entry=%#x\n", (u64)entry);

    SerialConsole.printf("end=%#x\n", (u64)end);

    while (entry + 2 <= end) {
        u8 type   = entry[0];
        u8 length = entry[1];

        SerialConsole.printf("entry=%#x type=%#x length=%#x\n", (u64)entry, type, length);

        // Every MADT entry must have at least a type + length.
        if (length < 2) {
            SerialConsole.puts("Invalid MADT entry length\n");
            break;
        }

        // Don't allow an entry to extend past the MADT.
        if (entry + length > end) {
            SerialConsole.puts("MADT entry extends past table\n");
            break;
        }

        if (type == 2 && length >= 10) {
            // Interrupt Source Override
            u8  source  = entry[3];
            u32 gsi     = rd32(entry + 4);

            SerialConsole.printf("ISO: IRQ=%#x GSI=%#x\n", source, gsi);

            if (source == target_irq)
                return gsi;
        }

        entry += length;
    }

    // No override found → identity mapping
    return target_irq;
}

void *find_madt_entry(MADT *madt, const u8 desired_entry) {
    // Pointer to the start of entries
    u8 *p = (u8 *)(madt + 1);

    // Caclulate the end of the table
    u8 *end = (u8 *)madt + madt->h.Length;

    while (p < end) {
        MADT_Entry *entry = (MADT_Entry *)p;
        u8 type = entry->type;
        u8 length = entry->length;

        if (type == desired_entry) {
            return (void *)p;
        }

        p += length;
    }

    return NULL;
}

void set_irqs() {
    XSDP_t *rsdp = (XSDP_t *)limine_parsed_data.rsdp_table_addr;

    void *xsdt = (void *)make_hhdm(rsdp->XsdtAddress);

    MADT *madt = (MADT *)find_acpi_table(xsdt, "APIC");

    SerialConsole.puts("About to run madt_get_gsi\n");
    u32 keyboard_gsi = madt_get_gsi(madt, 1);

    SerialConsole.puts("About to run ioapic_set_irq()\n");
    ioapic_set_irq(keyboard_gsi, IOAPICID, 0x21);
}

u64 get_ioapic_phys() {
    XSDP_t *rsdp = (XSDP_t *)limine_parsed_data.rsdp_table_addr;

    void *xsdt = (void *)make_hhdm(rsdp->XsdtAddress);

    MADT *madt = (MADT *)find_acpi_table(xsdt, "APIC");

    MADT_IOAPIC *ioapic = (MADT_IOAPIC *)find_madt_entry(madt, IOAPIC);

    return ioapic->ioapic_address;
}

u64 get_lapic_phys() {
    XSDP_t *rsdp = (XSDP_t *)limine_parsed_data.rsdp_table_addr;

    void *xsdt = (void *)make_hhdm(rsdp->XsdtAddress);

    MADT *madt = (MADT *)find_acpi_table(xsdt, "APIC");

    uintptr_t lapic_addr = madt->local_interrupt_controller_address;

    return lapic_addr;
}
