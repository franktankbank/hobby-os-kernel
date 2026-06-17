// #include "pm_timer.h"
// #include "../console.h"
// #include "../pmm.h"
// #include "table.h"
// #include "../util.h"
// #include "../globals.h"

// struct PMTimer get_pm_timer() {
//     uintptr_t rsdp_phys = from_hhdm(limine_parsed_data.rsdp_table_addr);

//     struct XSDP_t *rsdp = (struct XSDP_t *)rsdp_phys;

//     void *xsdt = (void *)rsdp->XsdtAddress;

//     struct FADT *fadt = find_acpi_table(xsdt, "FACP");

//     struct GenericAddressStructure X_PMTimerBlock = fadt->X_PMTimerBlock;

//     struct PMTimer pm_timer;

//     if (!(fadt->PMTimerLength == 4)) {
//         SerialConsole.printf("ACPI PM Timer is unavailable\n");
//         pm_timer.Available = false;
//         return pm_timer;
//     }

//     pm_timer.Available = true;

//     if (IS_BIT_SET(fadt->Flags, 8)) {
//         pm_timer.Bitness = 32;
//     } else {
//         pm_timer.Bitness = 24;
//     }

//     if (X_PMTimerBlock.Address == 0) {
//         pm_timer.Address32 = fadt->PMTimerBlock;
//         return pm_timer;
//     }

//     pm_timer.AddressSpace = X_PMTimerBlock.AddressSpace;

//     return pm_timer;
// }

// uint64_t read_pm_timer(struct PMTimer pm_timer) {

// }
