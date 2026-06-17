cmake_minimum_required(VERSION 4.3)

set(REQUIRED_CMDS
    "clang"
    "lld"
    "llvm-objcopy"
    "make"
    "dd"
    "sgdisk"
    "mformat"
    "mmd"
    "mcopy"
    "qemu-system-x86_64"
    "minicom"
)

foreach(REQUIRED_CMD ${REQUIRED_CMDS})
    string(REPLACE "-" "_" REQUIRED_CMD_SANITIZED ${REQUIRED_CMD})
    string(TOUPPER ${REQUIRED_CMD_SANITIZED} REQUIRED_CMD_UPPER)
    string(APPEND REQUIRED_CMD_UPPER "_EXE")

    find_program(${REQUIRED_CMD_UPPER} NAMES ${REQUIRED_CMD})

    if(NOT ${REQUIRED_CMD_UPPER})
        message(FATAL_ERROR "${REQUIRED_CMD} not found")
    endif()
endforeach()

set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "C compiler" FORCE)
set(CMAKE_LINKER_TYPE LLD)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    hobby-os-kernel
    LANGUAGES C ASM_NASM
)

include(uACPI/uacpi.cmake)
include(cmake/sources.cmake)
include(cmake/includes.cmake)
include(cmake/fonts.cmake)

set(CMAKE_C_FLAGS "-pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-PIC -ffunction-sections -fdata-sections -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -fdiagnostics-color=always" CACHE STRING "C compiler flags" FORCE)
set(CMAKE_ASM_NASM_FLAGS "-f elf64 -Wall -Xgnu" CACHE STRING "nasm assembler flags" FORCE)
set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g -F dwarf -O0" CACHE STRING "nasm assembler flags (DEBUG)" FORCE)
set(CMAKE_ASM_NASM_FLAGS_RELEASE "-Ox" CACHE STRING "nasm assembler flags (RELEASE)" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-m -Wl,elf_x86_64 -nostdlib -static -z max-page-size=0x1000 -Wl,--gc-sections -T ${CMAKE_CURRENT_SOURCE_DIR}/misc/linker.lds" CACHE STRING "linker flags" FORCE)

foreach(FONT_FILE ${FONT_FILES})
    get_filename_component(FONT_NAME ${FONT_FILE} NAME_WE)
    set(OBJ_FILE "${CMAKE_CURRENT_BINARY_DIR}/${FONT_NAME}.o")

    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O elf64-x86-64 -B i386 -I binary ${FONT_FILE} ${OBJ_FILE}
        DEPENDS ${FONT_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Converting ${FONT_FILE} to object file"
    )
    list(APPEND EMBEDDED_FONTS ${OBJ_FILE})
endforeach()

add_executable(kernel.elf)

target_sources(kernel.elf
    PRIVATE
        ${CPU_SOURCES}
        ${DRIVERS_SOURCES}
        ${KERNEL_SOURCES}
        ${UACPI_SOURCES}
        ${EMBEDDED_FONTS}
)

target_include_directories(kernel.elf
    PRIVATE
        ${CPU_INCLUDES}
        ${DRIVERS_INCLUDES}
        ${KERNEL_INCLUDES}
        ${UACPI_INCLUDES}
)

add_custom_target(create_dir
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/Limine
    VERBATIM
)

add_custom_target(limine
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/Limine/bootstrap
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/Limine/configure --enable-uefi-x86-64
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Limine
    VERBATIM
    DEPENDS create_dir
)

add_custom_target(image ALL
    COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/image.hdd
    COMMAND dd if=/dev/zero bs=1M count=0 seek=64 of=${CMAKE_CURRENT_BINARY_DIR}/image.hdd
    COMMAND sgdisk ${CMAKE_CURRENT_BINARY_DIR}/image.hdd -n 1:2048 -t 1:ef00 -m 1
    COMMAND mformat -i ${CMAKE_CURRENT_BINARY_DIR}/image.hdd@@1M
    COMMAND mmd -i ${CMAKE_CURRENT_BINARY_DIR}/image.hdd@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
    COMMAND mcopy -i ${CMAKE_CURRENT_BINARY_DIR}/image.hdd@@1M ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf ::/boot
    COMMAND mcopy -i ${CMAKE_CURRENT_BINARY_DIR}/image.hdd@@1M ${CMAKE_CURRENT_SOURCE_DIR}/misc/limine.conf ::/boot/limine
    COMMAND mcopy -i ${CMAKE_CURRENT_BINARY_DIR}/image.hdd@@1M ${CMAKE_CURRENT_BINARY_DIR}/Limine/bin/BOOTX64.EFI ::/EFI/BOOT
    DEPENDS limine
    VERBATIM
)

add_custom_target(start
    COMMAND qemu-system-x86_64 --bios OVMF.fd -drive file=${CMAKE_CURRENT_BINARY_DIR}/image.hdd,format=raw -chardev pty,path=/tmp/qemu-serial,id=serial0 -chardev pty,path=/tmp/qemu-monitor,id=monitor0 -serial chardev:serial0 -monitor chardev:monitor0 -cpu Skylake-Client,+x2apic
    VERBATIM
)

add_custom_target(serial
    COMMAND minicom -D /tmp/qemu-serial
    VERBATIM
)

add_custom_target(monitor
    COMMAND minicom -D /tmp/qemu-monitor
    VERBATIM
)
