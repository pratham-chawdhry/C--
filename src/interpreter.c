#include "../include/interpreter.h"
#include "../include/variable.h"
#include "../include/arithmetic.h"
#include "../include/print.h"
#include "../include/condition.h"
#include "../include/BODMAS.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE 100

bool result = true;

int* arr;
char* chararr;
int no_of_variables = 0;

int whitespaces(char* token){
    int spaces = 0;
    bool flag = true;
    for (int i = 0; i < strlen(token); i++){
        if (token[i] == ' '){
            if (flag) continue;
            else spaces++;
            flag = true;
        }
        else {
            flag = false;
            continue;
        }
    }
    return spaces;
}

int* arr_whitespaces(int spaces, char*token){
    int* arr = malloc(sizeof(int) * (spaces+2));
    bool flag = true;
    int index = 0;
    int j = 0;
    while (true){
        if (token[j] != ' '){
            arr[index++] = j;
            j++;
            break;
        }
        if (token[j+1] != ' '){
            arr[index++] = j;
            j++;
            break;
        }
        j++;
    }
    for (int i = j; i < strlen(token); i++){
        if (token[i] == ' '){
            if (flag && token[i+1] == ' ') continue;
            else if (token[i+1] != ' ') {
                arr[index++] = i;
            }
            flag = true;
        }
        else {
            flag = false;
            continue;
        }
    }
    arr[index++] = strlen(token);
    return arr;
}

char* slicing(int start,int stop, char* token){
    int i;
    char* str = (char*)malloc(sizeof(char) *(stop-start+1));
    for (i = start; i < stop; i++){
        if (token[i] == ' ') break;
        char c = token[i];
        str[i-start] = token[i];
    }
    str[i-start] = '\0';
    return str;
}

char* extractTextBetweenParentheses(const char *input) {
    const char *start = strchr(input, '(');
    const char *end = strchr(input, ')');
    char* text;

    if (start != NULL && end != NULL && start < end) {
        size_t length = end - start - 1;
        char extractedText[length + 1];
        strncpy(extractedText, start + 1, length);
        text = (char*)malloc(strlen(extractedText) + 1);
        extractedText[length] = '\0';
        strcpy(text,extractedText);
    } 
    else {
        exit(0);
    }
    return text;
}

void split(char* token) {
    int spaces = whitespaces(token);
    char** chararr = (char**)malloc(sizeof(char*) * (spaces + 2));
    
    int* arr = arr_whitespaces(spaces, token);
    char* str;

    int index = 0;
    int start = 0;
    while(true){
        if (!isalpha(token[0]) && token[0] != ' ' && token[0]!= '}'){
            printf("Can't begin with a non-alphabetic character: %s\n", token);
            exit(0);
        }
        if (token[0] != ' '){
            char* result = slicing(0, arr[1], token);
            str = (char*)malloc(sizeof(char) * (strlen(result) + 1));
            strcpy(str, result);
            break;
        }
        if (token[arr[index]] != ' '){
            char* result = slicing(arr[0]+1, arr[1], token);
            str = (char*)malloc(sizeof(char) * (strlen(result) + 1));
            strcpy(str, result);
            start = 1;
            break;
        }
        index++;
    }
    chararr[0] = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(chararr[0], str);
    free(str);


    for (int i = 1; i <= spaces; i++) {
        char* str = slicing(arr[i] + 1, arr[i+1], token);
        chararr[i] = (char*)malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(chararr[i], str);
        free(str);
    }

    if (strcmp(chararr[0], "}") == 0) {
        result = true;
    }

    if(result){
        for (int i = 0; i < spaces + 1; i++) {
            char *str_0 = "int\0";
            char *str_1 = "=\0";
            char *str_2 = "print\0";
            char *str_3 = "if\0";

            if (strcmp(chararr[i], str_0) == 0) {
                create_variable(spaces,chararr,no_of_variables);
                no_of_variables++;
            }
            else if (strcmp(chararr[i], str_1) == 0) {
                assign_variable(spaces,no_of_variables,token,chararr);
                break;
            }
            else if(strcmp(chararr[i], str_2) == 0) {
                print_variable(token,no_of_variables,chararr,spaces);
                break;
            }
            else if (strcmp(chararr[i], str_3) == 0) {
                char* equation = extractTextBetweenParentheses(token);
                int spaces_1 = whitespaces(equation);
                char** chararr_1 = (char**)malloc(sizeof(char*) * (spaces_1 + 2));

                int *arr_1 = arr_whitespaces(spaces_1, equation);
                char* str_1;

                int index_1 = 0;
                int start_1 = 0;

                while (true) {
                    if (!isalpha(equation[0]) && equation[0] != ' '){
                        printf("Can't begin with a non-alphabetic character: %s\n", equation);
                        exit(0);
                    }
                    if (equation[0] != ' '){
                        char* result_1 = slicing(0, arr_1[1], equation);
                        str_1 = (char*)malloc(sizeof(char) * (strlen(result_1) + 1));
                        strcpy(str_1, result_1);
                        break;
                    }
                    if (equation[arr_1[index_1]] != ' '){
                        char* result_1 = slicing(arr_1[0]+1, arr_1[1], equation);
                        str_1 = (char*)malloc(sizeof(char) * (strlen(result_1) + 1));
                        strcpy(str_1, result_1);
                        start_1 = 1;
                        break;
                    }
                    index_1 ++;
                }

                chararr_1[0] = (char*)malloc(sizeof(char) * (strlen(str_1) + 1));
                strcpy(chararr_1[0], str_1);
                free(str_1);

                for (int i = 1; i <= spaces_1; i++) {
                    char* str_2 = slicing(arr_1[i] + 1, arr_1[i+1], equation);
                    chararr_1[i] = (char*)malloc(sizeof(char) * (strlen(str_2) + 1));
                    strcpy(chararr_1[i], str_2);
                    free(str_2);
                }

                int l_value;

                if (isValidVariable(chararr_1[0])){
                    l_value = get_variable_value(chararr_1[0],no_of_variables);
                }
                else if(isNumber(chararr_1[0])){
                    l_value = atoi(chararr_1[0]);
                }

                char* operator = (char*)malloc(sizeof(char) * (strlen(chararr_1[1]) + 1));
                strcpy(operator, chararr_1[1]);

                int r_value;

                if (isValidVariable(chararr_1[2])){
                    r_value = get_variable_value(chararr_1[2],no_of_variables);
                }
                else if(isNumber(chararr_1[2])){
                    r_value = atoi(chararr_1[2]);
                }

                result = evaluate_condition(l_value, operator, r_value);
            }
            else if (strcmp(chararr[i], "}") == 0 && i == 0) {
                continue;
            }
        }
    }
}

void execute_c_minus_minus(char *code) {
    char* token = strtok(code, ";");
    int value = 0;

    while (token != NULL) {
        split(token);
        token = strtok(NULL, ";");
    }
}
