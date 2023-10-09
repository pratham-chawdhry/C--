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

int perform_arithmetic(int x, char op, int y){
    if (op == '+'){
        return x + y;
    }
    else if (op == '-'){
        return x - y;
    }
    else if (op == '*'){
        return x * y;
    }
    else if (op == '/'){
        return x / y;
    }
    else if (op == '%'){
        return x % y;
    }
    else{
        return INT_MAX;
    }
}

int eqn_spaces(char* token){
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

int* equation_eqn_spaces(int spaces, char*token){
    int* arr = malloc(sizeof(int) * (spaces+2));
    bool flag = true;
    int index = 0;

    arr[index++] = 0;
    for (int i = 1; i < strlen(token); i++){
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

char* modified_eqn_slicing(int start,int stop, char* token){
    int i;
    char* str = (char*)malloc(sizeof(char) *(stop-start+1));
    for (i = start; i < stop; i++){
        char c = token[i];
        str[i-start] = token[i];
    }
    str[i-start] = '\0';
    return str;
}

char* eqn_join(char **arr){
    char str[100] = "Hello\0";
    strcpy(str,arr[0]);
    char* space = " \0";
    strcat(str, space);

    int size = 0;

    while (arr[size] != NULL) {
        size++;
    }

    for (int i = 1; i < size; i++) {
        strcat(str, arr[i]);
        strcat(str, space);
    }
    char* result = malloc(strlen(str) + 1);
    strcpy(result,str);

    return result;
}

char* eqn_slicing(int start,int stop, char* token){
    int i;
    char* str = (char*)malloc(sizeof(char) *(stop-start+1));
    for (i = start; i < stop; i++){
        if (token[i] == ' ') break;
        char c = token[i];
        str[i-start] = token[i];
    }
    if (i > start) {
        str[i-start] = '\0';
        return str;
    }
    else{
        char* result = "\0";
        return result;
    }
}

int get_last_smaller(const int* arr, int size, int x) {
    int first = 0, last = size - 1;
    while (first <= last) {
        int mid = (first + last) / 2;
        if (arr[mid] >= x)
            last = mid - 1;
        else
            first = mid + 1;
    }
    return (first - 1 < 0) ? -1 : first - 1;
}

char* evaluate(char* token,char op) {
    int i;

    char equation[100] = "Hello\0";

    int length = eqn_spaces(token);
    int* arr = equation_eqn_spaces(length, token);

    for (i = 0; i < strlen(token); i++){
        if (token[i] == op) {
            int index  = get_last_smaller(arr, length, i);
            if (index == 1 && index == length){
                printf("%s ", token);
                char* str = "Bla\0";
                return str;
            }
            else if (index == 1){
                char* left_operand = eqn_slicing(arr[index-1], arr[index], token);
                char* right_operand = eqn_slicing(arr[index+1]+1, arr[index+2], token);
                int left = atoi(left_operand);
                int right = atoi(right_operand);
                int value = perform_arithmetic(left, op, right);

                char str[20];
                sprintf(str, "%d", value);
                strcpy(equation, str);
                char* right_equation = modified_eqn_slicing(arr[index+2], strlen(token), token);
                strcat(equation, right_equation);
                char* result = malloc(strlen(equation) + 1);
                strcpy(result,equation);
                return result;
            }
            else {
                char* left_operand = eqn_slicing(arr[index-1]+1, arr[index], token);
                char* right_operand = eqn_slicing(arr[index+1]+1, arr[index+2], token);
                int left = atoi(left_operand);
                int right = atoi(right_operand);
                int value = perform_arithmetic(left, op, right);

                char* left_equation = modified_eqn_slicing(0, arr[index-1] + 1, token);

                strcpy(equation, left_equation);
                char str[20];
                sprintf(str, "%d", value);
                strcat(equation, str);
                char* right_equation = modified_eqn_slicing(arr[index+2], strlen(token), token);
                strcat(equation, right_equation);
                char* result = malloc(strlen(equation) + 1);
                strcpy(result,equation);
                return result;
            }
        }
    }
}

bool isDone(char* token, char op) {
    for (int i = 0; i < strlen(token); i++){
        if (token[i] == op && token[i+1] == ' ') return false;
    }
    return true;
}

char* BODMAS(char* equation) {
    while(!isDone(equation,'/')){
        equation = evaluate(equation,'/');
    }
    while (!isDone(equation,'*')){
        equation = evaluate(equation,'*');
    }
    while (!isDone(equation,'+')){
        equation = evaluate(equation,'+');
    }
    while (!isDone(equation,'-')){
        equation = evaluate(equation,'-');
    }
    return equation;
}

char* corrected_equation(char* token){
    char equation[100] = "Hello\0";

    int i = 0;

    while (true){
        if (isalnum(token[i])){
            char str[2];
            str[0] = token[i];
            str[1] = '\0';

            strcpy(equation,str);
            //i++;
            break;
        }
        i++;
    }
    
    i++;
    
    bool space_flag = false;
    bool operator_flag = false;
    for (;i < strlen(token);i++){
        if (isalnum(token[i]) && isalnum(token[i-1])){
            char str[2];
            str[0] = token[i];
            str[1] = '\0';
            
            strcat(equation,str);
        }
        else if (isalnum(token[i]) && (token[i-1] == '/' || 
                token[i-1] == '*' || token[i-1] == '+' || 
                token[i-1] == '-')){
            char space[2] = " \0";
            strcat(equation,space);
            operator_flag = false;
            char str[2];
            str[0] = token[i];
            str[1] = '\0';
            
            strcat(equation,str);
        }
        else if ((token[i] == '/' || 
                token[i] == '*' || token[i] == '+' || 
                token[i] == '-') && isalnum(token[i-1])){
            char space[2] = " \0";
            strcat(equation,space);
            operator_flag = true;
            char str[2];
            str[0] = token[i];
            str[1] = '\0';
            
            strcat(equation,str);
        }
        else if (token[i] == ' ' && token[i-1] == ' '){
            continue;
        }
        else if (token[i] == ' ' && token[i-1] != ' '){
            char space[2] = " \0";
            strcat(equation,space);
        }
        else if ((isalnum(token[i]) || (token[i-1] == '/' || 
                token[i-1] == '*' || token[i-1] == '+' || 
                token[i-1] == '-')) && token[i-1] == ' '){
            char str[2];
            operator_flag = false;
            str[0] = token[i];
            str[1] = '\0';
            
            strcat(equation,str);
        }
        else if ((token[i] == '/' || 
                token[i] == '*' || token[i] == '+' || 
                token[i] == '-') && token[i-1] == ' ' && !operator_flag){
            char str[2];
            str[0] = token[i];
            str[1] = '\0';
            operator_flag = true;
            strcat(equation,str);
        }
        else if ((token[i] == '/' || 
                token[i] == '*' || token[i] == '+' || 
                token[i] == '-') && operator_flag){
            printf("Error : Invalid operator\n");
            exit(0);
        }
    }
    char* token_1 = (char*)malloc(strlen(equation)+1);
    strcpy(token_1,equation);
    return token_1;
}

// int main(){
//     char* token = " 20 + 6/           3";
//     char* equation = corrected_equation(token);
//     printf("%s\n", equation);
//     printf("%s\n", BODMAS(equation));
// }
//What if no left_operand or right operand, what if its 