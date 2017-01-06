#ifndef INCLUDE_DESCRIPTOR_TABLES_H
#define INCLUDE_DESCRIPTOR_TABLES_H

/** load_gdt:
 * Loads the GDT into the processor
 *
 * @param address The address of the GDT
 * @param size The size of the GDT
 */
void load_gdt(unsigned int address, unsigned short size);

#endif /* INCLUDE_DESCRIPTOR_TABLES_H */