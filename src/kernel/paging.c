#include "../types/int.h"
#include "acpi/table.h"
#include "console.h"
#include "pmm.h"
#include "paging.h"
#include "util.h"

static u64 *table_ptr(u64 phys) {
    return (u64 *)make_hhdm(phys);
}

u64 LAPIC_VIRT = 0xffff900000001000ULL;
u64 IOAPIC_VIRT = 0xFFFF900000000000ULL;

bool map_page(u64 pml4_phys, u64 virt, u64 phys, u64 flags) {
    u64 pml4_index = (virt >> 39) & 0x1FF;
    u64 pdpt_index = (virt >> 30) & 0x1FF;
    u64 pd_index   = (virt >> 21) & 0x1FF;
    u64 pt_index   = (virt >> 12) & 0x1FF;

    u64 *pml4 = table_ptr(pml4_phys);

    u64 pdpt_phys;
    if (!(pml4[pml4_index] & PTE_PRESENT)) {
        pdpt_phys = alloc_4k_page();

        if (!pdpt_phys)
            return false;

        memset(table_ptr(pdpt_phys), 0, PAGE_SIZE);

        pml4[pml4_index] =
            pdpt_phys |
            PTE_PRESENT |
            PTE_WRITE;
    } else {
        pdpt_phys = pml4[pml4_index] & PAGE_MASK;
    }

    u64 *pdpt = table_ptr(pdpt_phys);

    u64 pd_phys;
    if (!(pdpt[pdpt_index] & PTE_PRESENT)) {
        pd_phys = alloc_4k_page();

        if (!pd_phys)
            return false;

        memset(table_ptr(pd_phys), 0, PAGE_SIZE);

        pdpt[pdpt_index] =
            pd_phys |
            PTE_PRESENT |
            PTE_WRITE;
    } else {
        pd_phys = pdpt[pdpt_index] & PAGE_MASK;
    }

    u64 *pd = table_ptr(pd_phys);

    u64 pt_phys;
    if (!(pd[pd_index] & PTE_PRESENT)) {
        pt_phys = alloc_4k_page();

        if (!pt_phys)
            return false;

        memset(table_ptr(pt_phys), 0, PAGE_SIZE);

        pd[pd_index] =
            pt_phys |
            PTE_PRESENT |
            PTE_WRITE;
    } else {
        /*
         * If this is a 2 MiB huge-page mapping, we'd need
         * to split it before using a PT.
         */
        if (pd[pd_index] & PTE_PAGE_SIZE)
            return false;

        pt_phys = pd[pd_index] & PAGE_MASK;
    }

    u64 *pt = table_ptr(pt_phys);

    pt[pt_index] = (phys & PAGE_MASK) | flags;

    return true;
}

void setup_paging(void) {
    u64 old_cr3;

    asm volatile (
        "mov %%cr3, %0"
        : "=r"(old_cr3)
    );

    u64 old_pml4_phys = old_cr3 & PAGE_MASK;
    u64 *old_pml4 = (u64 *)make_hhdm(old_pml4_phys);

    u64 new_pml4_phys = alloc_4k_page();
    u64 *new_pml4 = (u64 *)make_hhdm(new_pml4_phys);

    memcpy(new_pml4, old_pml4, PAGE_SIZE);

    if (!map_page(
            new_pml4_phys,
            IOAPIC_VIRT,
            limine_parsed_data.ioapic_phys,
            PTE_PRESENT | PTE_WRITE
        )) {
        SerialConsole.puts("Failed to map IOAPIC\n");
        hcf();
    }

    if (!map_page(
        new_pml4_phys,
        LAPIC_VIRT,
        limine_parsed_data.lapic_phys,
        PTE_PRESENT | PTE_WRITE
    )) {
        SerialConsole.puts("Failed to map LAPIC\n");
        hcf();
    }

    asm volatile (
        "mov %0, %%cr3"
        :
        : "r"(new_pml4_phys)
        : "memory"
    );

    limine_parsed_data.ioapic_virt =
        (volatile u32 *)IOAPIC_VIRT;

    limine_parsed_data.lapic_virt =
        (volatile u32 *)LAPIC_VIRT;
}
