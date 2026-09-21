#include "../types/int.h"
#include "console.h"
#include "globals.h"
#include "pmm.h"
#include "util.h"

static u8 *pmm_bitmap;       // lives in physical RAM, accessed via HHDM
static u64 pmm_total_pages;  // total pages in the bitmap
static u64 pmm_free_count;

void pmm_init() {
    SerialConsole.printf("memmap count = %#llx\n",
        limine_parsed_data.memmap_entry_count);

    for (u64 i = 0; i < limine_parsed_data.memmap_entry_count; i++) {
        SerialConsole.printf("memmap[%#llx]: base=%#llx length=%#llx type=%#llx\n",
                i,
                limine_parsed_data.memmap_entries[i]->base,
                limine_parsed_data.memmap_entries[i]->length,
                limine_parsed_data.memmap_entries[i]->type);
    }

    // Find the highest usable address to size the bitmap
    u64 highest = 0;
    for (u64 i = 0; i < limine_parsed_data.memmap_entry_count; i++) {
        u64 end = limine_parsed_data.memmap_entries[i]->base
                     + limine_parsed_data.memmap_entries[i]->length;
        if (end > highest) highest = end;
    }

    pmm_total_pages = highest / PAGE_SIZE;
    // u64 bitmap_size = (pmm_total_pages + 7) / 8;

    // // Carve the bitmap out of the first usable region
    // for (u64 i = 0; i < limine_parsed_data.memmap_entry_count; i++) {
    //     if (limine_parsed_data.memmap_entries[i]->type == LIMINE_MEMMAP_USABLE
    //         && limine_parsed_data.memmap_entries[i]->length >= bitmap_size) {
    //         u64 base = limine_parsed_data.memmap_entries[i]->base;
    //         pmm_bitmap = (u8 *)make_hhdm(base); // virtual pointer via HHDM
    //         memset(pmm_bitmap, 0xFF, bitmap_size);         // all "used" initially
    //         // Shrink the usable region to account for the bitmap
    //         limine_parsed_data.memmap_entries[i]->base += bitmap_size;
    //         limine_parsed_data.memmap_entries[i]->length -= bitmap_size;
    //         break;
    //     }
    // }

    u64 bitmap_size = (pmm_total_pages + 7) / 8;
    u64 bitmap_pages = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    u64 bitmap_reserved = bitmap_pages * PAGE_SIZE;

    for (u64 i = 0; i < limine_parsed_data.memmap_entry_count; i++) {
        if (limine_parsed_data.memmap_entries[i]->type == LIMINE_MEMMAP_USABLE
            && limine_parsed_data.memmap_entries[i]->length >= bitmap_reserved) {

            u64 base = limine_parsed_data.memmap_entries[i]->base;
            u64 end = base + limine_parsed_data.memmap_entries[i]->length;

            pmm_bitmap = (u8 *)make_hhdm(base);
            memset(pmm_bitmap, 0xFF, bitmap_reserved);

            limine_parsed_data.memmap_entries[i]->base = base + bitmap_reserved;
            limine_parsed_data.memmap_entries[i]->length = end - (base + bitmap_reserved);

            break;
        }
    }

    // Mark all usable pages as free (bit = 0)
    pmm_free_count = 0;
    for (u64 i = 0; i < limine_parsed_data.memmap_entry_count; i++) {
        if (limine_parsed_data.memmap_entries[i]->type != LIMINE_MEMMAP_USABLE)
            continue;
        u64 start = limine_parsed_data.memmap_entries[i]->base / PAGE_SIZE;
        u64 count = limine_parsed_data.memmap_entries[i]->length / PAGE_SIZE;
        for (u64 j = 0; j < count; j++) {
            pmm_bitmap[(start + j) / 8] &= ~(1 << ((start + j) % 8));
            pmm_free_count++;
        }
    }

    SerialConsole.printf("PMM: total pages = %#llx\n", pmm_total_pages);
    SerialConsole.printf("PMM: free pages  = %#llx\n", pmm_free_count);
    SerialConsole.printf("PMM: bitmap      = %p\n", pmm_bitmap);
}

static u64 pmm_scan_index = 0; // hint for next allocation (bump allocator style)

u64 alloc_4k_page(void) {
    if (pmm_free_count == 0) return 0; // OOM

    for (u64 i = 0; i < pmm_total_pages; i++) {
        u64 idx = (pmm_scan_index + i) % pmm_total_pages;
        if (pmm_bitmap[idx / 8] & (1 << (idx % 8)))
            continue; // already used

        // Found a free page
        pmm_bitmap[idx / 8] |= (1 << (idx % 8));
        pmm_free_count--;
        pmm_scan_index = (idx + 1) % pmm_total_pages;
        return idx * PAGE_SIZE; // return PHYSICAL address
    }
    SerialConsole.printf("alloc_4k_page: free=%llu total=%llu\n",
        pmm_free_count, pmm_total_pages);
    return 0; // shouldn't reach here if free_count > 0
}
