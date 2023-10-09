#include <stdio.h>
#include "../include/interpreter.h"

int main() {
   char code[] =
        "int x = 10;"
        "int y = 5;"
        "int z = x + y;"
        "print z;"
        "if (z > 10) {"
        "    int w = 20;"
        "    print w;"
        "};";
    execute_c_minus_minus(code);
    return 0;
}