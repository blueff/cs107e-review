#include <shell.h>
#include <malloc.h>
#include <shell_commands.h>
#include <keyboard.h>
#include <strings.h>
#include <uart.h>
#include <pi.h>

#define LINE_MAX 80
#define ArrayCount(arr) (sizeof(arr) / sizeof(arr[0]))

static formatted_fn_t shell_printf;

static command_t commands[] = {
  {"help", "<cmd> prints a list of commands or description of cmd", cmd_help},
  {"echo", "<...> echos the user input to the screen", cmd_echo},
  {"reboot",
   "reboot the Raspberry Pi back to the bootloader using `pi_reboot",
   cmd_reboot},
  {"peek",
   "<address> prints the contents (4 bytes) of memory at address",
   cmd_peek},
  {"poke",
   "<address> <value> stores `value` into the memory at `address`",
   cmd_poke},
};

static command_t *
find_command(char *name) {
  for(int i = 0; i < ArrayCount(commands); i++) {
    if(strcmp(name, commands[i].name) == 0) {
      return commands + i;
    }
  }

  return 0;
}

static void
move_cursor_left(int n) {
  for(int i = 0; i < n; i++) { shell_printf("%c", '\b'); }
}

// ASCII escape sequence
static void
move_cursor_right(int n) {
  for(int i = 0; i < n; i++) { shell_printf("%c[C", 0x1b); }
}

int
cmd_echo(int argc, const char *argv[]) {
  for(int i = 1; i < argc; i++) shell_printf("%s ", argv[i]);
  shell_printf("\n");
  return 0;
}

int
cmd_help(int argc, const char *argv[]) {
  int result = 0;

  if(argc == 1) {
    for(int i = 0; i < ArrayCount(commands); i++) {
      shell_printf("%s: %s\n", commands[i].name, commands[i].description);
    }
  } else {
    command_t *cmd = find_command((char *)argv[1]);

    if(!cmd) {
      shell_printf("error: no such command `%s`.\n", argv[1]);
      result = 1;
    } else {
      shell_printf("%s: %s\n", cmd->name, cmd->description);
    }
  }

  return result;
}

int
cmd_reboot(int argc, const char *argv[]) {
  shell_printf("%c", EOT);
  pi_reboot();
  return 0;
}

int
cmd_peek(int argc, const char *argv[]) {
  if(argc < 2) {
    shell_printf("error: peek expects 1 argument [address]\n");
    return 1;
  }

  const char *endptr;
  unsigned int addr = strtonum(argv[1], &endptr);

  if(*endptr != 0) {
    shell_printf("error: peek cannot convert '%s'\n", argv[1]);
    return 1;
  }

  if(addr % 4 != 0) {
    shell_printf("error: peek address must be 4-byte aligned\n");
    return 1;
  }

  unsigned int value = *(unsigned int *)addr;
  shell_printf("%8x: %8x\n", addr, value);
  return 0;
}

int
cmd_poke(int argc, const char *argv[]) {
  if(argc < 3) {
    shell_printf("error: poke expects 2 arguments [address] [value]\n");
    return 1;
  }

  const char *endptr;
  unsigned int addr = strtonum(argv[1], &endptr);

  if(*endptr != 0) {
    shell_printf("error: poke cannot convert '%s'\n", argv[1]);
    return 1;
  }

  if(addr % 4 != 0) {
    shell_printf("error: poke address must be 4-byte aligned\n");
    return 1;
  }

  unsigned int value = strtonum(argv[2], &endptr);

  if(*endptr != 0) {
    shell_printf("error: poke cannot convert '%s'\n", argv[1]);
    return 1;
  }

  *(unsigned int *)addr = value;

  return 0;
}

void
shell_init(formatted_fn_t print_fn) {
  shell_printf = print_fn;
}

void
shell_bell(void) {
  shell_printf("%c", '\a');
}

void
shell_readline(char buf[], size_t bufsize) {
  size_t written = 0;
  int cursor = 0;

  while(1) {
    unsigned char next = keyboard_read_next();

    if(next == PS2_KEY_ARROW_LEFT && cursor > 0) {
      move_cursor_left(1);
      cursor--;
      continue;
    }

    if(next == PS2_KEY_ARROW_RIGHT && cursor < written) {
      move_cursor_right(1);
      cursor++;
      continue;
    }

    // ctrl-a
    if(next == 0x01) {
      move_cursor_left(cursor);
      cursor = 0;
      continue;
    }

    // ctrl-e
    if(next == 0x05) {
      move_cursor_right(written - cursor);
      cursor = written;
      continue;
    }

    // ctrl-u
    if(next == 0x15) {
      move_cursor_left(cursor);
      for(int i = 0; i < written; i++) { shell_printf(" "); }
      move_cursor_left(written);
      written = 0;
      cursor = 0;
      continue;
    }

    // Discard other special keys
    if(next > 0x7f) {
      continue;
    }

    if(next == '\n') {
      break;
    }

    if(next == '\b') {
      if(cursor > 0) {
        if(cursor == written) {
          written--;
          cursor--;
          move_cursor_left(1);
          shell_printf(" ");
          move_cursor_left(1);
        } else {
          move_cursor_left(1);
          int count = written - cursor;
          for(int i = 0; i < count; i++) {
            shell_printf("%c", buf[cursor + i]);
          }
          shell_printf(" ");
          move_cursor_left(count + 1);

          for(int i = cursor - 1; i < written - 1; i++) { buf[i] = buf[i + 1]; }
          cursor--;
          written--;
        }
      } else {
        shell_bell();
      }
      continue;
    }

    if(written == bufsize - 1) {
      shell_bell();
      continue;
    }

    if(cursor == written) {
      shell_printf("%c", next);
      buf[written] = next;
    } else {
      shell_printf("%c", next);
      int count = written - cursor;
      for(int i = 0; i < count; i++) { shell_printf("%c", buf[cursor + i]); }
      move_cursor_left(count);

      for(int i = written; i > cursor; i--) { buf[i] = buf[i - 1]; }
      buf[cursor] = next;
    }
    written++;
    cursor++;
  }

  shell_printf("\n");
  buf[written] = 0;
}

static char *
strndup(char *src, size_t n) {
  char *buf = malloc(n + 1);
  memcpy(buf, src, n);
  buf[n] = 0;
  return buf;
}

static inline int
isspace(char c) {
  return c == ' ' || c == '\t' || c == '\n';
}

static int
tokenize(const char *line, const char *array[], int max) {
  int ntokens = 0;
  char *cur = (char *)line;

  while(ntokens < max) {
    while(isspace(*cur)) cur++;
    if(*cur == 0)
      break;
    char *start = cur;
    while(*cur != 0 && !isspace(*cur)) cur++;
    array[ntokens++] = strndup(start, cur - start);
  }

  return ntokens;
}

int
shell_evaluate(const char *line) {
  // number of tokens must < LINE_MAX
  const char *tokens[LINE_MAX] = {};
  int argc = tokenize(line, tokens, LINE_MAX);

  if(argc == 0)
    return 0;

  command_t *cmd = find_command((char *)tokens[0]);

  if(!cmd) {
    shell_printf("error: no such command '%s'\n", tokens[0]);
    return 1;
  }

  int result = cmd->fn(argc, tokens);

  for(int i = 0; i < argc; i++) { free((char *)tokens[i]); }

  return result;
}

void
shell_run(void) {
  shell_printf(
    "Welcome to the CS107E shell. Remember to type on your PS/2 keyboard!\n");
  while(1) {
    char line[LINE_MAX];

    shell_printf("Pi> ");
    shell_readline(line, sizeof(line));
    shell_evaluate(line);
  }
}
