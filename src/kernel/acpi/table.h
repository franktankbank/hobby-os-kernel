#pragma once

#include "../macro.h"
#include "../../types/int.h"
#include <stdbool.h>

#define IOAPICID 0x00

// RSDP structure for ACPI revision 2
typedef struct {
    char Signature[8];
    u8 Checksum;
    char OEMID[6];
    u8 Revision;
    u32 RsdtAddress; // deprecated since version 2.0
    u32 Length;
    u64 XsdtAddress;
    u8 ExtendedChecksum;
    u8 reserved[3];
} PACKED XSDP_t;

typedef struct {
    char Signature[4];
    u32 Length;
    u8 Revision;
    u8 Checksum;
    char OEMID[6];
    char OEMTableID[8];
    u32 OEMRevision;
    u32 CreatorID;
    u32 CreatorRevision;
} PACKED ACPI_SDT_Header;

typedef struct {
    ACPI_SDT_Header h;
    u64 PointerToOtherSDT[];
} PACKED XSDT;

typedef struct {
    u8 AddressSpace;
    u8 BitWidth;
    u8 BitOffset;
    u8 AccessSize;
    u64 Address;
} PACKED Generic_Address_Structure;

typedef struct {
    ACPI_SDT_Header header;
    u32        FirmwareCtrl;
    u32        Dsdt;

    // field used in ACPI 1.0; no longer in use, for compatibility only
    u8         Reserved;

    u8         PreferredPowerManagementProfile;
    u16        SCI_Interrupt;
    u32        SMI_CommandPort;
    u8         AcpiEnable;
    u8         AcpiDisable;
    u8         S4BIOS_REQ;
    u8         PSTATE_Control;
    u32        PM1aEventBlock;
    u32        PM1bEventBlock;
    u32        PM1aControlBlock;
    u32        PM1bControlBlock;
    u32        PM2ControlBlock;
    u32        PMTimerBlock;
    u32        GPE0Block;
    u32        GPE1Block;
    u8         PM1EventLength;
    u8         PM1ControlLength;
    u8         PM2ControlLength;
    u8         PMTimerLength;
    u8         GPE0Length;
    u8         GPE1Length;
    u8         GPE1Base;
    u8         CStateControl;
    u16        WorstC2Latency;
    u16        WorstC3Latency;
    u16        FlushSize;
    u16        FlushStride;
    u8         DutyOffset;
    u8         DutyWidth;
    u8         DayAlarm;
    u8         MonthAlarm;
    u8         Century;

    // reserved in ACPI 1.0; used since ACPI 2.0+
    u16        BootArchitectureFlags;

    u8         Reserved2;
    u32        Flags;

    // 12 byte structure; see below for details
    Generic_Address_Structure   ResetReg;

    u8         ResetValue;
    u8         Reserved3[3];

    // 64bit pointers - Available on ACPI 2.0+
    u64                    X_FirmwareControl;
    u64                    X_Dsdt;

    Generic_Address_Structure   X_PM1aEventBlock;
    Generic_Address_Structure   X_PM1bEventBlock;
    Generic_Address_Structure   X_PM1aControlBlock;
    Generic_Address_Structure   X_PM1bControlBlock;
    Generic_Address_Structure   X_PM2ControlBlock;
    Generic_Address_Structure   X_PMTimerBlock;
    Generic_Address_Structure   X_GPE0Block;
    Generic_Address_Structure   X_GPE1Block;
} PACKED FADT;

typedef struct {
    u8 type;
    u8 length;
} PACKED MADT_Entry;

typedef struct {
    ACPI_SDT_Header h;
    u32 local_interrupt_controller_address;
    u32 flags;
    MADT_Entry entries[];
} PACKED MADT;

typedef struct {
    u8 type;   // 0
    u8 length; // 8
    u8 acpi_id;
    u8 apic_id;
    u32 flags; // bit 0 = Processor Enabled, bit 1 = Online Capable
} PACKED MADT_Local_APIC;

typedef struct {
    u8 type;   // 1
    u8 length; // 12
    u8 ioapic_id;
    u8 reserved;
    u32 ioapic_address;
    u32 global_system_interrupt_base;
} PACKED MADT_IOAPIC;

typedef struct {
    u8 type;   // 2
    u8 length; // 10
    u32 global_system_interrupt;
    u16 flags;
} PACKED MADT_IOAPIC_Interrupt_Source_Override;

typedef struct {
    u8 type;   // 3
    u8 length; // 10
    u8 nmi_source;
    u8 reserved;
    u16 flags;
    u32 global_system_interrupt;
} PACKED MADT_IOAPIC_Non_Maskable_Interrupt_Source;

typedef struct {
    u8 type;       // 4
    u8 length;     // 6
    u8 acpi_id;    // 0xFF means all processors
    u16 flags;
    u8 lint;       // 0 or 1
} PACKED MADT_Local_APIC_Non_Maskable_Interrupts;

typedef struct {
    u8 type;   // 5
    u8 length; // 12
    u16 reserved;
    u64 lapic_phys_addr;
} PACKED MADT_Local_APIC_Address_Override;

typedef struct {
    u8 type;   // 9
    u8 length; // 16
    u16 reserved;
    u32 x2apic_id;
    u32 flags;
    u32 acpi_id;
} PACKED MADT_Processor_Local_x2APIC;

enum MADT_Entries {
    LAPIC = 0,
    IOAPIC = 1,
    IOAPIC_INTERRUPT_SOURCE_OVERRIDE = 2,
    IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE = 3,
    LAPIC_NON_MASKABLE_INTERRUPTS = 4,
    LAPIC_ADDRESS_OVERRIDE = 5,
    PROCESSOR_LOCAL_X2APIC = 9
};

typedef union {
    struct {
        u64 vector : 8;
        u64 delvMode : 3;
        u64 destMode : 1;
        u64 delvStatus : 1;
        u64 pinPolarity : 1;
        u64 remoteIRR : 1;
        u64 triggerMode : 1;
        u64 mask : 1;
        u64 reserved : 39;
        u64 destination : 8;
    };
    struct {
        u32 lowerDword;
        u32 upperDword;
    };
} RedirectionEntry;

void *find_acpi_table(void *RootSDT, char signature[4]);

bool verify_checksum(ACPI_SDT_Header *tableHeader);

u64 get_lapic_phys();
u64 get_ioapic_phys();
void set_irqs();
