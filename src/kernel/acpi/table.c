#include "table.h"
#include "../globals.h"
#include "../pmm.h"
#include "../util.h"

void *find_acpi_table(void *RootSDT, char signature[4]) {
    struct XSDT *xsdt = (struct XSDT *) RootSDT;
    int entries = (xsdt->h.Length - sizeof(xsdt->h)) / 8;

    for (int i = 0; i < entries; i++) {
        struct ACPISDTHeader *h = (struct ACPISDTHeader *) make_hhdm(xsdt->PointerToOtherSDT[i]);

        if (!strncmp(h->Signature, signature, 4))
            return (void *) h;
    }

    // Table not found
    return NULL;
}

bool verify_checksum(struct ACPISDTHeader *tableHeader) {
    unsigned char sum = 0;

    for (uint32_t i = 0; i < tableHeader->Length; i++) {
        sum += ((char *) tableHeader)[i];
    }

    return sum == 0;
}

uint32_t get_lapic_addr() {
    struct XSDP_t *rsdp = (struct XSDP_t *)limine_parsed_data.rsdp_table_addr;

    void *xsdt = (void *)make_hhdm(rsdp->XsdtAddress);

    struct MADT *madt = (struct MADT *)find_acpi_table(xsdt, "APIC");

    uintptr_t lapic_addr = madt->local_interrupt_controller_address;

    return lapic_addr;
}
