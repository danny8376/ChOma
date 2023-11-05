#ifndef MACHO_SLICE_H
#define MACHO_SLICE_H

#include <stdbool.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include "MemoryStream.h"
#include "FAT.h"

typedef struct MachOSegment
{
    struct segment_command_64 command;
    struct section_64 sections[];
} __attribute__((__packed__)) MachOSegment;

typedef struct FilesetMachO {
    char *entry_id;
    uint64_t vmaddr;
    uint64_t fileoff;
	FAT underlyingMachO;
} FilesetMachO;

typedef struct MachO {
    MemoryStream stream;
    bool isSupported;
    struct mach_header_64 machHeader;
    struct fat_arch_64 archDescriptor;

    uint32_t filesetCount;
    FilesetMachO *filesetMachos;

    uint32_t segmentCount;
    MachOSegment **segments;
} MachO;

// Read data from a MachO at a specified offset
int macho_read_at_offset(MachO *macho, uint64_t offset, size_t size, void *outBuf);

int macho_enumerate_load_commands(MachO *macho, void (^enumeratorBlock)(struct load_command loadCommand, uint32_t offset, void *cmd, bool *stop));

// Initialise a MachO object from a MemoryStream and it's corresponding FAT arch descriptor
int macho_init(MachO *macho, MemoryStream *stream, struct fat_arch_64 archDescriptor);

void macho_free(MachO *macho);

#endif // MACHO_SLICE_H