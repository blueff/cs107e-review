#include <uart.h>
#include <stdint.h>
#include "../printf.h"

typedef union {
  uint32_t value;
  struct  {
    uint32_t cond:4;
    uint32_t op:2;
  };
} arm_ins;

typedef union {
  uint32_t value;
  struct {
    uint32_t shifter_operand:12;
    uint32_t rd:4;
    uint32_t rn:4;
    uint32_t status:1;
    uint32_t cmd:4;
    uint32_t immediate:1;
    uint32_t :2; // 00
    uint32_t cond:4;
  };
} data_processing_ins;

char *register_names[16] = {
  "r0",
  "r1",
  "r2",
  "r3",
  "r4",
  "r5",
  "r6",
  "r7",
  "r8",
  "r9",
  "r10",
  "r11",
  "r12",
  "sp",
  "fp",
  "pc",
};

uint32_t right_rotate(uint32_t n, uint32_t d)
{
  return (n >> d) | (n << (32 - d));
}

uint32_t calc_immediate_value(uint32_t number) {
  uint32_t result;
  uint32_t rotate = (number >> 8) & 0b1111;
  uint32_t value = number & 0xff;
  result = right_rotate(value, rotate * 2);
  return result;
}

void disassemble_data_processing(char *buf, size_t bufsize, data_processing_ins ins) {
  switch(ins.cmd) {
    // MOV
    case 0b1101: {
      if(ins.immediate) {
        uint32_t v = calc_immediate_value(ins.shifter_operand);
        snprintf(
          buf, bufsize,
          "mov %s, #0x%x",
          register_names[ins.rd],
          v
        );
      } else {
        int c = snprintf(buf, bufsize, "mov --- ");
        buf += c;
        bufsize -= c;
        goto tmp;
      }
    } break;

    default: {
tmp:
      snprintf(
        buf, bufsize,
        "immediate: %d, cmd: %4b, status: %d, rn: %d, rd: %d, shifter_operand: %b",
        ins.immediate, ins.cmd, ins.status, ins.rn, ins.rd,
        ins.shifter_operand
      );
    } break;
  }
}

void disassemble(char *buf, size_t bufsize, arm_ins instruction) {
  switch(instruction.op) {
    // data processing
    case 0: {
      data_processing_ins ins;
      ins.value = instruction.value;
      disassemble_data_processing(buf, bufsize, ins);
    } break;

    default: {
      snprintf(buf, bufsize, "---");
    };
  }
}

void main(void) {
  unsigned int addr = 0x8000;
  unsigned int *p = (unsigned int *)addr;
  arm_ins ins;

  char buf[1024];

  for(int i = 0; i < 100; i++) {
    ins.value = *p++;

    disassemble(buf, sizeof(buf), ins);

    printf("%x: %8x %s\n", addr, ins.value, buf);

    addr += 4;
  }

  uart_putchar(EOT);
}