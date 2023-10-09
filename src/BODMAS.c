#include "../include/arithmetic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

#define MAX_SIZE 100 

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;


void initializeStack(Stack* stack) {
    stack->top = -1; 
}


int isEmpty(Stack* stack) {
    return stack->top == -1;
}


int isFull(Stack* stack) {
    return stack->top == MAX_SIZE - 1;
}


void push(Stack* stack, int value) {
    if (isFull(stack)) {
        printf("Stack overflow! Cannot push %d\n", value);
        return; 
    }
    
    stack->data[++stack->top] = value; 
}


int pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow! Cannot pop from an empty stack\n");
        return -1; 
    }
    
    return stack->data[stack->top--];
}

bool isBracket(char* modified_str){
    int open_bracket_count = 0;
    int close_bracket_count = 0;
    for (int i = 0; i < strlen(modified_str); i++){
        if (modified_str[i] == '('){
            open_bracket_count++;
        }
        else if (modified_str[i] == ')'){
            close_bracket_count++;
        }
    }
    if (open_bracket_count != close_bracket_count){
        printf("Unbalanced brackets: %s\n", modified_str);
    }
    else if (open_bracket_count == 0){
        return false;
    }
    else{
        return true;
    }
}

char* sol(int length, char* modified_str){
    int value, i;

    Stack stack;
    initializeStack(&stack);
    for (i = 0; i < length; i++) {
        if (modified_str[i] == '(') push(&stack, i);
        else if (modified_str[i] == ')') {
            value = pop(&stack);
            char* left_operand = modified_eqn_slicing(0, value, modified_str);
            char* str_1 = modified_eqn_slicing(value + 1, i, modified_str);
            char* right_operand = modified_eqn_slicing(i + 1, length, modified_str);
            
            if (strlen(left_operand) != 0) {
                strcpy(modified_str, left_operand);
                char* result = (char*)malloc((strlen(str_1) + 1) * sizeof(char));
                strcpy(result, str_1);
                result = corrected_equation(result);
                result = BODMAS(result);
                strcat(modified_str,result);
            } 
            else {
                char* result = (char*)malloc((strlen(str_1) + 1) * sizeof(char));
                strcpy(result, str_1);
                result = corrected_equation(result);
                result = BODMAS(result);
                strcpy(modified_str, result);
            }
            
            if (strlen(right_operand) != 0) {
                strcat(modified_str, right_operand);
            }
            
            free(left_operand);
            free(str_1);
            free(right_operand);
            break;
        }
    }

    return modified_str;
}

char* helper(char* str) {
    int value;

    int str_length = strlen(str);
    
    char* modified_str = (char*)malloc((str_length + 1) * sizeof(char));
    
    strcpy(modified_str, str);
    int i;

    bool flag = true;

    while (isBracket(modified_str)) {
        modified_str = sol(str_length, modified_str);
    }

    modified_str = corrected_equation(modified_str);
    modified_str = BODMAS(modified_str);

    return modified_str;
}

// int main(){
//     char* str = " (20 / (4) * 3 - 1 * 2)";
//     helper(str);
// }
