#pragma once

#define PTE_PRESENT         (1 << 0)    /* bit 0 */
#define PTE_WRITE           (1 << 1)    /* bit 1 */
#define PTE_USER_SUPERVISOR (1 << 2)    /* bit 2 */
#define PTE_WRITE_THROUGH   (1 << 3)    /* bit 3 */
#define PTE_CACHE_DISABLE   (1 << 4)    /* bit 4 */
#define PTE_ACCESSED        (1 << 5)    /* bit 5 */
#define PTE_DIRTY           (1 << 6)    /* bit 6 */
#define PTE_PAGE_SIZE       (1 << 7)    /* bit 7 */
#define PTE_GLOBAL          (1 << 8)    /* bit 8 */
#define PTE_NOEXEC          (1 << 63)   /* bit 63*/

#define PAGE_MASK 0x000FFFFFFFFFF000ULL

void setup_paging();
