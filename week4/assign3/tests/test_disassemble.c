#include "../assert.h"
#include "../strings.h"
#include "../disassemble.h"

void main(void) {
  char buf[100];

  // mov
  {
    disassemble(buf, sizeof(buf), 0xe3a0d302, 0);
    assert(strcmp(buf, "mov sp, #134217728") == 0);

    disassemble(buf, sizeof(buf), 0xe3a0b000, 0);
    assert(strcmp(buf, "mov fp, #0") == 0);

    disassemble(buf, sizeof(buf), 0xe3a0b000, 0);
    assert(strcmp(buf, "mov fp, #0") == 0);

    disassemble(buf, sizeof(buf), 0x21a0300a, 0);
    assert(strcmp(buf, "movcs r3, sl") == 0);

    disassemble(buf, sizeof(buf), 0x01b01062, 0);
    assert(strcmp(buf, "rrxseq r1, r2") == 0);

    disassemble(buf, sizeof(buf), 0x01b01162, 0);
    assert(strcmp(buf, "rorseq r1, r2, #2") == 0);
  }

  // data processing
  {
    disassemble(buf, sizeof(buf), 0xe2855004, 0);
    assert(strcmp(buf, "add r5, r5, #4") == 0);

    disassemble(buf, sizeof(buf), 0x00009b74, 0);
    assert(strcmp(buf, "andeq r9, r0, r4, ror fp") == 0);

    disassemble(buf, sizeof(buf), 0x00109b78, 0);
    assert(strcmp(buf, "andseq r9, r0, r8, ror fp") == 0);

    disassemble(buf, sizeof(buf), 0xe1530002, 0);
    assert(strcmp(buf, "cmp r3, r2") == 0);

    disassemble(buf, sizeof(buf), 0x20200000, 0);
    assert(strcmp(buf, "eorcs r0, r0, r0") == 0);

    disassemble(buf, sizeof(buf), 0xe3c2260e, 0);
    assert(strcmp(buf, "bic r2, r2, #14680064") == 0);

    disassemble(buf, sizeof(buf), 0xe3822602, 0);
    assert(strcmp(buf, "orr r2, r2, #2097152") == 0);

    disassemble(buf, sizeof(buf), 0xe064a00a, 0);
    assert(strcmp(buf, "rsb sl, r4, sl") == 0);

    // bx/teq
    {
      disassemble(buf, sizeof(buf), 0xe13fff1e, 0);
      assert(strcmp(buf, "teq pc, lr, lsl pc") == 0);

      disassemble(buf, sizeof(buf), 0xe12fff1e, 0);
      assert(strcmp(buf, "bx lr") == 0);
    }
  }

  // branch
  {
    disassemble(buf, sizeof(buf), 0xeb000020, 0);
    assert(strcmp(buf, "bl 0x88") == 0);

    disassemble(buf, sizeof(buf), 0xeafffffe, 0);
    assert(strcmp(buf, "b 0x0") == 0);

    disassemble(buf, sizeof(buf), 0xeafffffd, 0);
    assert(strcmp(buf, "b 0xfffffffc") == 0);
  }

  // push/pop
  {
    disassemble(buf, sizeof(buf), 0xe92d43f0, 0);
    assert(strcmp(buf, "push {r4, r5, r6, r7, r8, r9, lr}") == 0);

    disassemble(buf, sizeof(buf), 0xe8bd43f0, 0);
    assert(strcmp(buf, "pop {r4, r5, r6, r7, r8, r9, lr}") == 0);

    disassemble(buf, sizeof(buf), 0xe92d4008, 0);
    assert(strcmp(buf, "push {r3, lr}") == 0);

    disassemble(buf, sizeof(buf), 0xe8bd4008, 0);
    assert(strcmp(buf, "pop {r3, lr}") == 0);
  }
}