#ifndef INTERPRETER_H
#define INTERPRETER_H

int whitespaces(char* token);
int* arr_whitespaces(int spaces, char*token);
char* slicing(int start,int stop, char* token);
void execute_c_minus_minus(char *code);
void split(char* token);

#endif /* INTERPRETER_H */
