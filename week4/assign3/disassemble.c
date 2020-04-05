#include <stdbool.h>
#include <stdint.h>
#include <uart.h>

#include "assert.h"
#include "printf.h"

typedef union {
  uint32_t raw;
  struct {
    uint32_t : 25;
    uint32_t main : 3;
    uint32_t cond : 4;
  };
} arm_ins;

typedef union {
  uint32_t raw;
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
  "sl",
  "fp",
  "r12",
  "sp",
  "lr",
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

inline uint32_t
right_rotate(uint32_t n, uint32_t d) {
  return (n >> d) | (n << (32 - d));
}

inline uint32_t
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
disassemble_mov(
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
      setStatus,
      cond,
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
      setStatus,
      cond,
      get_register_name(ins.rd),
      get_register_name(ins.shifter_operand));
    return;
  }

  shifter_operand_params params = parse_shifter_operand(ins.shifter_operand);
  int mode = params.mode;
  int rm = params.rm;
  int imm = params.imm;
  int rs = params.rs;

  char *shift_types[] = {"lsl", "lsr", "asr", "ror"};
  char *shift_type = shift_types[mode >> 1];

  switch(mode) {
    case 0b000: // lsl by immediate
    case 0b010: // lsr by immediate
    case 0b100: // asr by immediate
    case 0b110: // ror by immediate
    {
      // ror,rrx
      if(mode == 0b110 && imm == 0) {
        snprintf(
          buf,
          bufsize,
          "rrx%s%s %s, %s",
          setStatus,
          cond,
          get_register_name(ins.rd),
          get_register_name(rm));
      } else {
        snprintf(
          buf,
          bufsize,
          "%s%s%s %s, %s, #%d",
          shift_type,
          setStatus,
          cond,
          get_register_name(ins.rd),
          get_register_name(rm),
          imm);
      }
    } break;

    case 0b001: // lsl by register
    case 0b011: // lsr by register
    case 0b101: // asr by register
    case 0b111: // ror by register
    {
      snprintf(
        buf,
        bufsize,
        "%s%s%s %s, %s, %s",
        shift_type,
        setStatus,
        cond,
        get_register_name(ins.rd),
        get_register_name(rm),
        get_register_name(rs));
    } break;

    default: {
      invalid_code_path;
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
      disassemble_mov(buf, bufsize, ins);
    } break;

    case 0b0001: // eor
    case 0b0100: // add
    case 0b0000: // and
    case 0b0010: // sub
    case 0b0011: // rsb
    case 0b1110: // bic
    case 0b1100: // orr
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
        setStatus,
        cond,
        get_register_name(ins.rd),
        get_register_name(ins.rn),
        operand);
    } break;

    // TEQ/BX
    case 0b1001: {
      // TEQ
      if(ins.status) {
        char *cond = get_cond_name(ins.cond);

        char operand[1024];
        format_operand(
          operand, sizeof(operand), ins.immediate, ins.shifter_operand);

        snprintf(
          buf,
          bufsize,
          "teq%s %s, %s",
          cond,
          get_register_name(ins.rn),
          operand);
      } else { // BX
        snprintf(
          buf,
          bufsize,
          "bx %s",
          get_register_name(ins.shifter_operand & 0b1111)
        );
      }
    } break;

    case 0b1010: // cmp
    {
      char *cond = get_cond_name(ins.cond);

      char *operation = get_operation_name(ins.operation);

      char operand[1024];
      format_operand(
        operand, sizeof(operand), ins.immediate, ins.shifter_operand);

      snprintf(
        buf,
        bufsize,
        "%s%s %s, %s",
        operation,
        cond,
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

inline uint32_t
calc_target_addr(uint32_t addr, int32_t offset) {
  uint32_t result;
  bool is_signed = (offset >> 23) == 1;
  if(is_signed) {
    offset = (0b111111 << 24) | offset;
  }
  offset <<= 2;
  result = addr + offset + 8;
  return result;
}

void disassemble_branch(
  char *buf,
  size_t bufsize,
  uint32_t raw,
  uint32_t addr) {
  char *cond = get_cond_name(raw >> 28);
  bool is_link = raw & (1 << 24);
  uint32_t target_addr = calc_target_addr(addr, raw & 0xffffff);

  snprintf(
    buf,
    bufsize,
    "b%s%s 0x%x",
    is_link ? "l" : "",
    cond,
    target_addr
  );
}

inline void
format_register_list(char *buf, size_t bufsize, uint32_t list) {
  char *p = buf;
  char *end = buf + bufsize;
  bool is_first = true;
  p += snprintf(p, end - p, "{");

  for(int i = 0; i <= 15; i++) {
    if(list & (1 << i)) {
      if(is_first) {
        is_first = false;
      } else {
        p += snprintf(p, end - p, ", ");
      }
      p += snprintf(p, end - p, "%s", get_register_name(i));
    }
  }

  snprintf(p, end - p, "}");
}

void
disassemble_load_store_multiple(char *buf, size_t bufsize, uint32_t raw) {
  bool p = (raw >> 24) & 0x1;
  bool u = (raw >> 23) & 0x1;
  bool s = (raw >> 22) & 0x1;
  bool w = (raw >> 21) & 0x1;
  bool l = (raw >> 20) & 0x1;
  int rn = (raw >> 16) & 0xf;

  char tmp[1024];
  format_register_list(tmp, sizeof(tmp), raw & 0xffff);

  // push
  if(p == 1 && u == 0 && s == 0 && w == 1 && l == 0 && rn == 13) {
    snprintf(
      buf,
      bufsize,
      "push %s",
      tmp
    );
  }

  // pop
  else if(p == 0 && u == 1 && s == 0 && w == 1 && l == 1 && rn == 13) {
    snprintf(
      buf,
      bufsize,
      "pop %s",
      tmp
    );
  }

  else {
    snprintf(
      buf,
      bufsize,
      "!!![load/store multiple] p: %d, u: %d, s: %d, w: %d, l: %d, rn: %d",
      p, u, s, w, l, rn
    );
  }
}

void
disassemble(
  char *buf,
  size_t bufsize,
  uint32_t raw,
  uint32_t addr) {
  arm_ins ins;
  ins.raw = raw;

  // data processing
  if((ins.main >> 2) == 0) {
    data_processing_ins d;
    d.raw = ins.raw;
    disassemble_data_processing(buf, bufsize, d);
  }

  // branch
  else if(ins.main == 0b101) {
    disassemble_branch(buf, bufsize, raw, addr);
  }

  // load/store multiple
  else if(ins.main == 0b100) {
    disassemble_load_store_multiple(buf, bufsize, raw);
  }

  else {
    snprintf(buf, bufsize, "---");
  }
}
