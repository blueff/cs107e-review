#include <stdbool.h>
#include <stdint.h>
#include <uart.h>

#include "../assert.h"
#include "../printf.h"

typedef union {
  uint32_t value;
  struct {
    uint32_t : 26;
    uint32_t op : 2;
    uint32_t cond : 4;
  };
} arm_ins;

typedef union {
  uint32_t value;
  struct {
    uint32_t shifter_operand : 12;
    uint32_t rd : 4;
    uint32_t rn : 4;
    uint32_t status : 1;
    uint32_t operation : 4;
    uint32_t immediate : 1;
    uint32_t : 2; // 00
    uint32_t cond : 4;
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
inline char *
get_register_name(uint32_t num) {
  assert(num <= 15);
  return register_names[num];
}

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
  ""  // 0b1111
};
inline char *
get_cond_name(uint32_t value) {
  assert(value <= 15);
  return cond_names[value];
}

char *operation_name[] = {
  "and",
  "eor",
  "sub",
  "rsb",
  "add",
  "adc",
  "sbc",
  "rsc",
  "tst",
  "teq",
  "cmp",
  "cmn",
  "orr",
  "mov",
  "bic",
  "mvn",
};
inline char *
get_operation_name(uint32_t index) {
  assert(index <= 15);
  return operation_name[index];
}

typedef struct shifter_operand_params {
  uint32_t mode;
  uint32_t rm;
  uint32_t imm;
  uint32_t rs;
  bool is_register;
} shifter_operand_params;

shifter_operand_params
parse_shifter_operand(uint32_t value) {
  shifter_operand_params result = {};

  result.is_register = (value >> 4) == 0;
  result.mode = (value >> 4) & 0b111;
  result.rm = value & 0b1111;
  result.imm = value >> 7;
  result.rs = value >> 8;

  return result;
}

uint32_t
right_rotate(uint32_t n, uint32_t d) {
  return (n >> d) | (n << (32 - d));
}

uint32_t
calc_immediate_value(uint32_t number) {
  uint32_t result;
  uint32_t rotate = (number >> 8) & 0b1111;
  uint32_t value = number & 0xff;
  result = right_rotate(value, rotate * 2);
  return result;
}

void
format_operand(
  char *buf,
  size_t bufsize,
  bool immediate,
  uint32_t shifter_operand) {
  if(immediate) {
    uint32_t v = calc_immediate_value(shifter_operand);
    snprintf(buf, bufsize, "#%d", v);
    return;
  }

  shifter_operand_params params = parse_shifter_operand(shifter_operand);
  uint32_t mode = params.mode;
  uint32_t imm = params.imm;

  // register value
  if(params.is_register) {
    snprintf(buf, bufsize, "%s", get_register_name(shifter_operand));
  }

  char *rm_name = get_register_name(params.rm);
  char *rs_name = get_register_name(params.rs);

  char *shift_types[] = {"lsl", "lsr", "asr", "ror"};
  char *shift_type = shift_types[mode >> 1];

  switch(mode) {
    case 0b000: // lsl by immediate
    case 0b010: // lsr by immediate
    case 0b100: // asr by immediate
    case 0b110: // ror by immediate
    {
      if(imm == 0) {
        if(mode == 0b110) { // Rotate right with extend
          snprintf(buf, bufsize, "%s, rrx", rm_name);
        } else {
          snprintf(buf, bufsize, "%s", rm_name);
        }
      } else {
        snprintf(buf, bufsize, "%s, %s #%d", rm_name, shift_type, imm);
      }
    } break;

    case 0b001: // lsl by register
    case 0b011: // lsr by register
    case 0b101: // asr by register
    case 0b111: // ror by register
    {
      snprintf(buf, bufsize, "%s, %s %s", rm_name, shift_type, rs_name);
    } break;

    default: {
      snprintf(buf, bufsize, "!!!operand");
    } break;
  }
}

void
disassemble_data_processing_mov(
  char *buf,
  size_t bufsize,
  data_processing_ins ins) {
  char *setStatus = ins.status ? "s" : "";
  char *cond = get_cond_name(ins.cond);

  if(ins.immediate) {
    uint32_t v = calc_immediate_value(ins.shifter_operand);
    snprintf(
      buf,
      bufsize,
      "mov%s%s %s, #%d",
      cond,
      setStatus,
      get_register_name(ins.rd),
      v);
    return;
  }

  // register value
  if((ins.shifter_operand >> 4) == 0) {
    snprintf(
      buf,
      bufsize,
      "mov%s%s %s, %s",
      cond,
      setStatus,
      get_register_name(ins.rd),
      get_register_name(ins.shifter_operand));
    return;
  }

  shifter_operand_params params = parse_shifter_operand(ins.shifter_operand);
  int mode = params.mode;
  int rm = params.rm;
  int imm = params.imm;
  int rs = params.rs;

  switch(mode) {
    // lsl by immediate
    case 0b000: {
      snprintf(
        buf,
        bufsize,
        "lsl%s%s %s, %s, #%d",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        imm);
    } break;

    // lsl by register
    case 0b001: {
      snprintf(
        buf,
        bufsize,
        "lsl%s%s %s, %s, %s",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        get_register_name(rs));
    } break;

    // lsr by immediate
    case 0b010: {
      snprintf(
        buf,
        bufsize,
        "lsr%s%s %s, %s, #%d",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        imm);
    } break;

    // lsr by register
    case 0b011: {
      snprintf(
        buf,
        bufsize,
        "lsr%s%s %s, %s, %s",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        get_register_name(rs));
    } break;

    // asr by immediate
    case 0b100: {
      snprintf(
        buf,
        bufsize,
        "asr%s%s %s, %s, #%d",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        imm);
    } break;

    // asr by register
    case 0b101: {
      snprintf(
        buf,
        bufsize,
        "asr%s%s %s, %s, %s",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        get_register_name(rs));
    } break;

    // ror by immediate
    case 0b110: {
      // rrx
      if(imm == 0) {
        snprintf(
          buf,
          bufsize,
          "rrx%s%s %s, %s",
          cond,
          setStatus,
          get_register_name(ins.rd),
          get_register_name(rm));
      } else {
        snprintf(
          buf,
          bufsize,
          "ror%s%s %s, %s, #%d",
          cond,
          setStatus,
          get_register_name(ins.rd),
          get_register_name(rm),
          imm);
      }
    } break;

    // ror by register
    case 0b111: {
      snprintf(
        buf,
        bufsize,
        "ror%s%s %s, %s, %s",
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(rm),
        get_register_name(rs));
    } break;

    default: {
      snprintf(buf, bufsize, "!!![mov] %b, mode: %b", ins.value, mode);
    } break;
  }
}

void
disassemble_data_processing(
  char *buf,
  size_t bufsize,
  data_processing_ins ins) {
  switch(ins.operation) {
    // mov
    case 0b1101: {
      disassemble_data_processing_mov(buf, bufsize, ins);
    } break;

    case 0b0001: // eor
    case 0b0100: // add
    case 0b0000: // and
    case 0b0010: // sub
    {
      char *setStatus = ins.status ? "s" : "";
      char *cond = get_cond_name(ins.cond);

      char *operation = get_operation_name(ins.operation);

      char operand[1024];
      format_operand(
        operand, sizeof(operand), ins.immediate, ins.shifter_operand);

      snprintf(
        buf,
        bufsize,
        "%s%s%s %s, %s, %s",
        operation,
        cond,
        setStatus,
        get_register_name(ins.rd),
        get_register_name(ins.rn),
        operand);
    } break;

    case 0b1010: // cmp
    {
      char *setStatus = ins.status ? "s" : "";
      char *cond = get_cond_name(ins.cond);

      char *operation = get_operation_name(ins.operation);

      char operand[1024];
      format_operand(
        operand, sizeof(operand), ins.immediate, ins.shifter_operand);

      snprintf(
        buf,
        bufsize,
        "%s%s%s %s, %s",
        operation,
        cond,
        setStatus,
        get_register_name(ins.rn),
        operand);
    } break;

    default: {
      snprintf(
        buf,
        bufsize,
        "!!![data processing] immediate: %d, operation: %4b, status: "
        "%d, rn: %d, rd: %d, shifter_operand: %b",
        ins.immediate,
        ins.operation,
        ins.status,
        ins.rn,
        ins.rd,
        ins.shifter_operand);
    } break;
  }
}

void
disassemble(char *buf, size_t bufsize, uint32_t value) {
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

void
main(void) {
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