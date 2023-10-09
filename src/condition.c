#include "../include/interpreter.h"
#include "../include/variable.h"
#include "../include/arithmetic.h"
#include "../include/print.h"
#include "../include/condition.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

// int sentinel = 1;

bool evaluate_condition(int x, char *op, int y){
    if (strcmp(op, "==") == 0){
        return x == y;
    }
    else if (strcmp(op, "!=") == 0){
        return x != y;
    }
    else if (strcmp(op, "<") == 0){
        return x < y;
    }
    else if (strcmp(op, ">") == 0){
        return x > y;
    }
    else if (strcmp(op, "<=") == 0){
        return x <= y;
    }
    else if (strcmp(op, ">=") == 0){
        return x >= y;
    }
    return false;
}

int condition_slicing(char* code,int no_of_variables){
    char* token = strtok(code, " ");
    int l_value;
    int r_value;

    if (isValidVariable(token)){
        l_value = get_variable_value(token,no_of_variables);
        printf("%d\n", l_value);
    }
    else if(isNumber(token)){
        l_value = atoi(token);
        printf("%d\n", l_value);
    }

    token = strtok(NULL, " ");
    printf("%s\n", token);
    char* operator = (char*)malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(operator, token);

    token = strtok(NULL, " ");
    if (isValidVariable(token)){
        r_value = get_variable_value(token,no_of_variables);
        printf("%d\n", r_value);
    }
    else if(isNumber(token)){
        r_value = atoi(token);
        printf("%d\n", r_value);
    }

    bool result = evaluate_condition(l_value, operator, r_value);
    int myint = result;
    return myint;
}