#include "../include/interpreter.h"
#include "../include/variable.h"
#include "../include/arithmetic.h"
#include "../include/print.h"
#include "../include/condition.h"
#include "../include/BODMAS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

int isNumber(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10); 
    return endptr != str && *endptr == '\0';
}

int isOperator(const char *str) {
   return strcmp(str, "+") == 0 || strcmp(str, "-") == 0 || strcmp(str, "*") == 0 || strcmp(str, "/") == 0 || strcmp(str, "%") == 0;
}

void print_variable(char *token, int no_of_variables,char** chararr,int spaces){
    char* str = "print\0";

    int i;
    int value;

    for (i = 0; i < spaces + 1; i++){
        if (strcmp (chararr[i], str) == 0){
            break;
        }
    }

    if (i == spaces){
        printf("Nothing to print: %s?\n", token);
        exit(0);
    }

    char algebra[100] = "Hello\0";
    char *null_c = "\0";
    char *space = " \0";
    if (i + 1 <= spaces && (strcmp(chararr[i+1],null_c))!=0){
        if (isNumber(chararr[i + 1])){
            strcpy(algebra,chararr[i + 1]);
        }
        else if (chararr[i+1][0] == '('){
            char* bracket = "(\0";
            strcpy(algebra,bracket);
            char* rest = modified_eqn_slicing(1,strlen(chararr[i+1]),chararr[i+1]);

            if (isValidVariable(rest)){
                value = get_variable_value(rest,no_of_variables);
                sprintf(algebra,"%s%d",algebra, value);
            }
            else if (isNumber(rest)){
                strcat(algebra,rest);
            }
            else {
                printf("Invalid type : %s\n", rest);
                exit(0);
            }
        }
        else if (isValidVariable(chararr[i+1])){
            value = get_variable_value(chararr[i+1],no_of_variables);
            sprintf(algebra,"%d", value);
        }
        else {
            printf("Undefined print command : %s\n", token);
            exit(0);
        }
        strcat(algebra,space);
        i = i + 2;
    }
    else {
        printf("Nothing to print: %s?\n", chararr[i]);
        exit(0);
    }

    for (int j = i; j < spaces + 1; j++){
        if (isNumber(chararr[j])){
            strcat(algebra,chararr[j]);
            strcat(algebra,space);
        }
        else if (isOperator(chararr[j])){
            strcat(algebra,chararr[j]);
            strcat(algebra,space);
        }
        else if (chararr[j][0] == '('){
            char* bracket = "(\0";
            strcat(algebra,bracket);
            char* rest = modified_eqn_slicing(1,strlen(chararr[j]),chararr[j]);

            if (isValidVariable(rest)){
                value = get_variable_value(rest,no_of_variables);
                sprintf(algebra,"%s%d",algebra, value);
            }
            else if (isNumber(rest)){
                strcat(algebra,rest);
            }
            else {
                printf("Invalid type : %s\n", rest);
                exit(0);
            }
        }
        else if (chararr[j][strlen(chararr[j]) - 1] == ')'){
            char* bracket = ")\0";
            char* rest = modified_eqn_slicing(0,strlen(chararr[j])-1,chararr[j]);

            if (isValidVariable(rest)){
                value = get_variable_value(rest,no_of_variables);
                sprintf(algebra,"%s%d",algebra, value);
            }
            else if (isNumber(rest)){
                strcat(algebra,rest);
            }
            else {
                printf("Invalid type : %s\n", rest);
                exit(0);
            }
            strcat(algebra,bracket);
        }
        else if (isValidVariable(chararr[j])){
            value = get_variable_value(chararr[j],no_of_variables);
            sprintf(algebra,"%s%d",algebra,value);
            strcat(algebra,space);
        }
    }

    //printf("Algebra : %s\n", algebra);
    char* result = (char*)malloc(sizeof(char) * (strlen(algebra) + 1));
    strcpy(result,algebra);
    result = helper(result);

    printf("%s\n", result);
}