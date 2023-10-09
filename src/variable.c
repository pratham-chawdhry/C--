#include "../include/interpreter.h"
#include "../include/variable.h"
#include "../include/arithmetic.h"
#include "../include/print.h"
#include "../include/condition.h"
#include "../include/BODMAS.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NO_OF_VARIABLES 100

struct vars{
    char* name;
    char* values;
};

struct vars myvars[MAX_NO_OF_VARIABLES];

bool isValidVariable(char* input) {
    if (strlen(input) == 0) {
        return false;
    }
    if (!isalpha(input[0]) && input[0] != '_') {
        return false;
    }
    for (int i = 1; i < strlen(input); i++) {
        if (!isalnum(input[i]) && input[i] != '_') {
            return false;
        }
    }
    return true;
}

bool isnotInitialised(int no_of_variables, char* variableName) {
    for (int i = 0; i < no_of_variables; i++) {
        if (strcmp(myvars[i].name, variableName) == 0) {
            return false;
        }
    }
    return true;
}
 
void create_variable(int spaces,char **chararr,int no_of_variables){
    char* str = "int\0";
    char* new_var;
    for (int i = 0; i < spaces + 1; i++){
        if (strcmp (chararr[i], str) == 0){
            new_var = (char*)malloc(sizeof(char) * (strlen(chararr[i+1]) + 1));
            strcpy(new_var, chararr[i+1]);
            if (isValidVariable(new_var) && isnotInitialised(no_of_variables, new_var)) {
                myvars[no_of_variables].name = new_var;
                myvars[no_of_variables].values = NULL;
            }
            else if (!isValidVariable(new_var)){
                printf("Invalid variable name: %s\n", new_var);
                exit(0);
            }
            else if (!isnotInitialised(no_of_variables, new_var)){
                printf("Variable already initialised: %s\n", new_var);
                exit(0);
            }
            break;
        }
    }
    //printf("%s ", myvars[0].name);
}

// void assign_variable(int spaces, char **chararr, int no_of_variables){
void assign_variable(int spaces,int no_of_variables,char* token,char** chararr){
    char* equal = "=\0";
    char* new_var;
    bool flag = false;
    int i;

    for (i = 0; i < spaces + 1; i++){
        if (strcmp (chararr[i], equal) == 0){
            break;
        }
        // printf("%s ", chararr[i]);
    }

    if (i > 0 && isnotInitialised(no_of_variables, chararr[i-1])){
            printf("Variable not initialised: %s\n", chararr[i-1]);
            exit(0);
    }

    //printf("%s ", chararr[i+1]);
    int value;

    int variable_index = i - 1;
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

    // printf("%s", algebra);
    // printf("%s",chararr[i]);

    for (int j = i; j < spaces + 1; j++){
        //printf("%s ", chararr[j]);
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
        // printf("%s\n", algebra);
    }

    //printf("%s\n", algebra);
    char* result = (char*)malloc(sizeof(char) * (strlen(algebra) + 1));
    strcpy(result,algebra);
    result = helper(result);
    // result = BODMAS(result);

    // for (int k = i; k < spaces + 1; k++){
    //     if (isValidVariable(chararr[k])){
    //         int value = get_variable_value(chararr[k],no_of_variables);
    //     }
    //     strcat(algebra,chararr[k]);
    //     strcat(algebra,space);
    // }

    // char* result = (char*)malloc(sizeof(char) * (strlen(algebra) + 1));
    // strcpy(result,algebra);
    // result = BODMAS(result);

    for (int i = 0; i < no_of_variables; i++){
        if (strcmp(myvars[i].name, chararr[variable_index]) == 0){
            myvars[i].values = (char*)malloc(sizeof(char) * (strlen(result) + 1));
            strcpy(myvars[i].values, result);
            break;
        }
    }

    // printf(" The answer is %s\n", result);
}

int get_variable_value(char *var_name,int no_of_variables){
    char* str = "int\0";
    char* str_1 = "if\0";
    if (strcmp(var_name, str) == 0 || strcmp(var_name, str_1) == 0){
        printf("%s is a keyword. Invalid variable name: %s\n", var_name, var_name);
        exit(0);
    }
    if (strlen(var_name) == 0  || !isValidVariable(var_name)){
        printf("Invalid variable name: %s\n", var_name);
        exit(0);
    }
    for (int i = 0; i < no_of_variables; i++){
        if (strcmp(myvars[i].name, var_name) == 0){
            if (myvars[i].values == NULL){
                printf("Variable has no value: %s\n", var_name);
                exit(0);
            }
            // printf("%s\n", myvars[i].name);
            return atoi(myvars[i].values);
        }
    }
    return -1;
}

// char* eqn_join(char **arr){
//     char str[100] = "Hello\0";
//     strcpy(str,arr[0]);
//     char* space = " \0";
//     strcat(str, space);

//     int size = 0;

//     while (arr[size] != NULL) {
//         size++;
//     }

//     for (int i = 1; i < size; i++) {
//         strcat(str, arr[i]);
//         strcat(str, space);
//     }
//     char* result = malloc(strlen(str) + 1);
//     strcpy(result,str);

//     return result;
// }