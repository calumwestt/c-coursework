#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "display.h"

enum {DX, DY, PEN};

struct state {
  int x;
  int y;
  int dx;
  int dy;
  int dt;
  int count;
  long operand;
  bool pen; //true is down, false is up.
  display *display;
};
typedef struct state state;

state *new_state(display *d) {
  state *s = malloc(sizeof(state));
  s -> x = 0;
  s -> y = 0;
  s -> dx = 0;
  s -> dy = 0;
  s -> dt = 0;
  s -> count = 0;
  s -> operand = 0;
  s -> pen = false;
  s -> display = d;
  return s;
}

int get_opcode(unsigned char code) {
  return code >> 6;
}

int get_operand(unsigned char code) {
  return (code & 0x3F);
}

void extend_sign(int operand, state *s) {
  s -> operand = (s -> operand << 6) | operand;
  s -> count++;
}

int check_sign(state *s) {
  long temp = 1 << (6 * (s -> count) - 1);
  if ((s -> operand & temp) != 0) return s -> operand - (temp * 2);
  else return s -> operand;
}

void draw(state *s) {
  if (s -> pen == true) {
    line(s -> display, s -> x, s -> y, (s -> x + s -> dx), (s -> y + s -> dy));
  }
  s -> x = s -> x + s -> dx;
  s -> y = s -> y + s -> dy;
  s -> dx = 0;
  s -> dy = 0;
  s -> count = 0;
  s -> operand = 0;
}

void set_extended_instruction(int new_opcode, state *s) {
  if (new_opcode == 0) {
    if (s -> pen == false) s -> pen = true;
    else s -> pen = false;
  }
  else if (new_opcode == 1) {
    if (s -> dt == 0) s -> dt = s -> operand;
    pause(s -> display, s -> dt);
    s -> operand = 0;
  }
  else if (new_opcode == 2) {
    clear(s -> display);
    s -> count = 0;
  }
  else if (new_opcode == 3) {
    key(s -> display);
  }
  else if (new_opcode == 4) {
    colour(s -> display, s -> operand);
    s -> count = 0;
    s -> operand = 0;
  }
}

void set_instruction(int opcode, int operand, state *s) {
  if (opcode == 0) {
    extend_sign(operand, s);
    s -> operand = check_sign(s);
    s -> dx = s -> operand;
    s -> count = 0;
    s -> operand = 0;
  }
  else if (opcode == 1) {
    extend_sign(operand, s);
    s -> operand = check_sign(s);
    s -> dy = s -> operand;
    draw(s);
  }
  else if (opcode == 2) {
    extend_sign(operand, s);
  }
  else if (opcode ==  3) {
    set_extended_instruction(operand, s);
  }
}

void translate_byte(unsigned char b, state *s) {
  int opcode = get_opcode(b);
  int operand = get_operand(b);
  set_instruction(opcode, operand, s);
}

void read_n_run(char file[], display *d) {
  state *s = new_state(d);
  FILE *in = fopen(file, "rb");
  unsigned char b = fgetc(in);
  while (! feof(in)) {
    translate_byte(b, s);
    b = fgetc(in);
  }
  fclose(in);
}

//------------------------------------------------------------------------------

void test_opcode() {
  assert(get_opcode(0x03) == 0);
  assert(get_opcode(0x7d) == 1);
  assert(get_opcode(0xc0) == 3);
}

void test_operand() {
  assert(get_operand(0x03) == 3);
  assert(get_operand(0xc0) == 0);
}

void test_new_display() {
  display *d = newDisplay("test", 200, 200);
  assert(d != NULL);
  free(d);
}

void test_new_state() {
  display *d = newDisplay("test", 200, 200);
  state *s = new_state(d);
  assert(s != NULL);
  free(d);
  free(s);
}

void run_test() {
  test_opcode();
  test_operand();
  test_new_display();
  test_new_state();
  printf("All tests pass.\n");
}

int main(int n, char *args[n]) {
  if (n == 2) {
    display *d = newDisplay(args[1], 200, 200);
    read_n_run(args[1], d);
    end(d);
  }
  else {
    run_test();
  }
  return 0;
}
