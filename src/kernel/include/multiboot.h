/*  multiboot.h - Multiboot header file.  */
/*  Copyright (C) 1999,2003,2007,2008,2009  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MULTIBOOT_H
#define MULTIBOOT_H


#include <stdint.h>

#define MULTIBOOT_HEADER_MAGIC              0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC          0x2BADB002
#define MULTIBOOT_UNSUPPORTED               0x0000FFFC
#define MULTIBOOT_MOD_ALIGN                 0x00001000
#define MULTIBOOT_INFO_ALIGN                0x00000004

/* multiboot header */
#define MULTIBOOT_PAGE_ALIGN                0x00000001
#define MULTIBOOT_MEMORY_INFO               0x00000002
#define MULTIBOOT_VIDEO_MODE                0x00000004
#define MULTIBOOT_AOUT_KLUDGE               0x00010000

/* multiboot info structure */
#define MULTIBOOT_INFO_MEMORY               0x00000001
#define MULTIBOOT_INFO_BOOTDEV              0x00000002
#define MULTIBOOT_INFO_CMDLINE              0x00000004
#define MULTIBOOT_INFO_MODS                 0x00000008
#define MULTIBOOT_INFO_AOUT_SYMS            0x00000010
#define MULTIBOOT_INFO_ELF_SHDR             0X00000020
#define MULTIBOOT_INFO_MEM_MAP              0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO           0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE         0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER_NAME     0x00000200
#define MULTIBOOT_INFO_APM_TABLE            0x00000400
#define MULTIBOOT_INFO_VIDEO_INFO           0x00000800

struct multiboot_header
{
  uint32_t magic;
  uint32_t flags;
  uint32_t checksum;

  uint32_t header_addr;
  uint32_t load_addr;
  uint32_t load_end_addr;
  uint32_t bss_end_addr;
  uint32_t entry_addr;

  uint32_t mode_type;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
};

typedef struct multiboot_aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
} multiboot_aout_symbol_table_t;

typedef struct multiboot_elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
} multiboot_elf_section_header_table_t;

typedef struct multiboot_info
{
  uint32_t flags;

  uint32_t mem_lower;
  uint32_t mem_upper;

  uint32_t boot_device;

  uint32_t cmdline;

  uint32_t mods_count;
  uint32_t mods_addr;

  union
  {
    multiboot_aout_symbol_table_t aout_sym;
    multiboot_elf_section_header_table_t elf_sec;
  } u;

  uint32_t mmap_length;
  uint32_t mmap_addr;

  uint32_t drives_length;
  uint32_t drives_addr;

  uint32_t config_table;

  uint32_t boot_loader_name;

  uint32_t apm_table;

  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
} multiboot_info_t;

struct multiboot_mmap_entry
{
  uint32_t size;
  uint64_t addr;
  uint64_t len;
  uint32_t type;
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

typedef struct multiboot_mod_list
{
  uint32_t mod_start;
  uint32_t mod_end;
  uint32_t cmdline;
  uint32_t pad;
} multiboot_module_t;


#endif // MULTIBOOT_H
