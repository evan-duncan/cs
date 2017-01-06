#include "descriptor_tables.h"

struct gdt
{
  unsigned int address;
  unsigned short size;
} __attribute__((packed));
