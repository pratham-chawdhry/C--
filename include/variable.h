#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>

bool isValidVariable(char* input);
void create_variable(int spaces,char **chararr,int no_of_variables);
void assign_variable(int spaces,int no_of_variables,char* token,char** chararr);
int get_variable_value(char *var_name,int no_of_variables);

#endif /* VARIABLE_H */
