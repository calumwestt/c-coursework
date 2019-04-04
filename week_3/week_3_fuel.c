/* Calculate the amount of fuel used for a car journey and how much that will cost. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

// Checks validity of user input. //
bool valid(const char length[], long num, char *ptr) {
  if (length[0] == '0') return false;
  if (num > INT_MAX | num <= 0) return false;
  if (*ptr == '\0' | *ptr == '\n') return true;
  else return false;
}

// Converts the user input into a double format. //
double convert(const char length[]) {
    char *ptr;
    double num;
    num = strtol(length, &ptr, 10);
    if (valid(length, num, ptr)) return num;
    else return -1;
}

// Calculates the total cost of the trip. //
double calculate(double distance, double mpg, double fuel_cost) {
    double trip_cost;
    const double gal_lit = 4.546;
    trip_cost = ((((distance / mpg) * gal_lit) * fuel_cost) / 100);
    return trip_cost;
}

// Runs the program, if no arguments are provided then tests are run. //
int main(int n, char *args[n]) {
    if (n == 4) {
      double distance = convert(args[1]);
      double mpg = convert(args[2]);
      double fuel_cost = convert(args[3]);
      double trip_cost = calculate(distance, mpg, fuel_cost);
      printf("The cost of your journey will be: £""%.2f\n", trip_cost);
    }
    else {
      printf("Please supply these arguments: Distance (miles), MPG, Fuel price (p)\n");
    }
    return 0;
}
