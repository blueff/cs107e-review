#include <uart.h>
#include <stdint.h>
#include "../printf.h"
#include "../assert.h"

typedef union {
  uint32_t value;
  struct  {
    uint32_t :26;
    uint32_t op:2;
    uint32_t cond:4;
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

char *cond_names[16] = {
  "eq",
  "ne",
  "cs",
  "cc",
  "mi",
  "pl",
  "vs",
  "vc",
  "hi",
  "ls",
  "ge",
  "lt",
  "gt",
  "le",
  "", // always
  "" // 0b1111
};

uint32_t right_rotate(uint32_t n, uint32_t d) {
  return (n >> d) | (n << (32 - d));
}

uint32_t calc_immediate_value(uint32_t number) {
  uint32_t result;
  uint32_t rotate = (number >> 8) & 0b1111;
  uint32_t value = number & 0xff;
  result = right_rotate(value, rotate * 2);
  return result;
}

inline char *
get_cond_name(uint32_t value) {
  assert(value <= 15);
  return cond_names[value];
}

void disassemble_data_processing_mov(char *buf, size_t bufsize, data_processing_ins ins) {

  char *setStatus = ins.status ? "s": "";
  char *cond = get_cond_name(ins.cond);

  if(ins.immediate) {
    uint32_t v = calc_immediate_value(ins.shifter_operand);
    snprintf(
      buf, bufsize,
      "mov%s%s %s, #%d",
      cond,
      setStatus,
      register_names[ins.rd],
      v
    );
    return;
  }

  // register value
  if((ins.shifter_operand >> 4) == 0) {
    snprintf(
      buf, bufsize,
      "mov%s%s %s, %s",
      cond,
      setStatus,
      register_names[ins.rd],
      register_names[ins.shifter_operand]
    );
    return;
  }

  int mode = (ins.shifter_operand >> 4) & 0b111;
  int rm = ins.shifter_operand & 0b1111;
  int imm = ins.shifter_operand >> 7;
  int rs = ins.shifter_operand >> 8;

  switch(mode) {
    // lsl by immediate
    case 0b000: {
      snprintf(
        buf, bufsize,
        "lsl%s%s %s, %s, #%d",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        imm
      );
    } break;

    // lsl by register
    case 0b001: {
      snprintf(
        buf, bufsize,
        "lsl%s%s %s, %s, %s",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        register_names[rs]
      );
    } break;

    // lsr by immediate
    case 0b010: {
      snprintf(
        buf, bufsize,
        "lsr%s%s %s, %s, #%d",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        imm
      );
    } break;

    // lsr by register
    case 0b011: {
      snprintf(
        buf, bufsize,
        "lsr%s%s %s, %s, %s",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        register_names[rs]
      );
    } break;

    // asr by immediate
    case 0b100: {
      snprintf(
        buf, bufsize,
        "asr%s%s %s, %s, #%d",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        imm
      );
    } break;

    // asr by register
    case 0b101: {
      snprintf(
        buf, bufsize,
        "asr%s%s %s, %s, %s",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        register_names[rs]
      );
    } break;

    // ror by immediate
    case 0b110: {
      // rrx
      if(imm == 0) {
        snprintf(
          buf, bufsize,
          "rrx%s%s %s, %s",
          cond,
          setStatus,
          register_names[ins.rd],
          register_names[rm]
        );
      } else {
        snprintf(
          buf, bufsize,
          "ror%s%s %s, %s, #%d",
          cond,
          setStatus,
          register_names[ins.rd],
          register_names[rm],
          imm
        );
      }
    } break;

    // ror by register
    case 0b111: {
      snprintf(
        buf, bufsize,
        "ror%s%s %s, %s, %s",
        cond,
        setStatus,
        register_names[ins.rd],
        register_names[rm],
        register_names[rs]
      );
    } break;

    default: {
      snprintf(buf, bufsize, "!!![mov] %b, mode: %b", ins.value, mode);
    } break;
  }
}

void disassemble_data_processing(char *buf, size_t bufsize, data_processing_ins ins) {
  switch(ins.cmd) {
    // MOV
    case 0b1101: {
      disassemble_data_processing_mov(buf, bufsize, ins);
    } break;

    default: {
      break;
      snprintf(
        buf, bufsize,
        "!!![data processing] immediate: %d, cmd: %4b, status: %d, rn: %d, rd: %d, shifter_operand: %b",
        ins.immediate, ins.cmd, ins.status, ins.rn, ins.rd,
        ins.shifter_operand
      );
    } break;
  }
}

void disassemble(char *buf, size_t bufsize, uint32_t value) {
  arm_ins ins;
  ins.value = value;

  switch(ins.op) {
    // data processing
    case 0: {
      data_processing_ins d;
      d.value = ins.value;
      disassemble_data_processing(buf, bufsize, d);
    } break;

    default: {
      snprintf(buf, bufsize, "---");
    };
  }
}

void main(void) {
  uint32_t addr = 0x8000;
  uint32_t *p = (uint32_t *)addr;
  uint32_t value;

  char buf[1024];

  for(int i = 0; i < 100; i++) {
    value = *p++;

    disassemble(buf, sizeof(buf), value);

    printf("%x: %8x %s\n", addr, value, buf);

    addr += 4;
  }

  uart_putchar(EOT);
}