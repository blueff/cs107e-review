#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include <stdint.h>
#include <stddef.h>

void disassemble(char *buf, size_t bufsize, uint32_t raw, uint32_t addr);

#endif