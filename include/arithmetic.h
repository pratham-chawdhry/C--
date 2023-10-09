#ifndef ARITHMETIC_H
#define ARITHMETIC_H

int perform_arithmetic(int x, char op, int y);
int eqn_spaces(char* token);
int* equation_eqn_spaces(int spaces, char*token);
char* modified_eqn_slicing(int start,int stop, char* token);
char* eqn_join(char **arr);
char* eqn_slicing(int start,int stop, char* token);
int get_last_smaller(const int* arr, int size, int x);
char* evaluate(char* token,char op);
//bool isDone(char* token, char op);
char* BODMAS(char* equation);
char* corrected_equation(char* equation);

#endif /* ARITHMETIC_H */
