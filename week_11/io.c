#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void write_line(FILE *out, char *line) {
  fprintf(out, "%s", convert(line));
}

void convert(char *line) {

}

void read_line(FILE *in, FILE *out) {
  char line[100];
  fgets(line, 100, in);
  while (! feof(in)) {
    write_line(out, line);
    fgets(line, 100, in);
  }
  fclose(in);
  fclose(out);
}

int main(int n, char *args[n]) {
  if (n == 3) {
    FILE *in = fopen(args[1], "r");
    FILE *out = fopen(args[2], "w");
    read_line(in, out);
  }
  else {
    printf("Please enter two filepaths, first is input, second is output\n");
  }
}
