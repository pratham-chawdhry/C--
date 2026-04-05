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

int no_of_variables = 0;

static char* skip_ws(char* p) {
    while (*p && isspace((unsigned char)*p)) p++;
    return p;
}

static char* get_block(char** pp) {
    char* p = *pp;
    p = skip_ws(p);
    if (*p != '{') return NULL;
    p++;
    char* start = p;
    int depth = 1;
    while (*p && depth > 0) {
        if (*p == '{') depth++;
        else if (*p == '}') depth--;
        p++;
    }
    if (depth > 0) {
        fprintf(stderr, "Error: Unbalanced braces\n");
        exit(1);
    }
    size_t len = (p - 1) - start;
    char* block = malloc(len + 1);
    memcpy(block, start, len);
    block[len] = '\0';
    *pp = p;
    return block;
}

static char* get_parentheses(char** pp) {
    char* p = *pp;
    p = skip_ws(p);
    if (*p != '(') return NULL;
    p++;
    char* start = p;
    int depth = 1;
    while (*p && depth > 0) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        p++;
    }
    if (depth > 0) {
        fprintf(stderr, "Error: Unbalanced parentheses\n");
        exit(1);
    }
    size_t len = (p - 1) - start;
    char* content = malloc(len + 1);
    memcpy(content, start, len);
    content[len] = '\0';
    *pp = p;
    return content;
}

static bool eval_cond_str(char* cond) {
    char s1[128], op[16], s2[128];
    char* p = cond;
    while (*p && isspace((unsigned char)*p)) p++;

    char* s = p;
    while (*p && (isalnum((unsigned char)*p) || *p == '_')) p++;
    int len = p - s;
    if (len >= 128) len = 127;
    memcpy(s1, s, len);
    s1[len] = '\0';

    while (*p && isspace((unsigned char)*p)) p++;
    s = p;
    while (*p && (*p == '=' || *p == '!' || *p == '<' || *p == '>')) p++;
    len = p - s;
    if (len >= 16) len = 15;
    memcpy(op, s, len);
    op[len] = '\0';

    while (*p && isspace((unsigned char)*p)) p++;
    s = p;
    while (*p && (isalnum((unsigned char)*p) || *p == '_')) p++;
    len = p - s;
    if (len >= 128) len = 127;
    memcpy(s2, s, len);
    s2[len] = '\0';

    if (!s1[0] || !op[0] || !s2[0]) return false;

    int v1, v2;
    if (isdigit((unsigned char)s1[0]) || (s1[0] == '-' && isdigit((unsigned char)s1[1]))) v1 = atoi(s1);
    else v1 = get_variable_value(s1, no_of_variables);

    if (isdigit((unsigned char)s2[0]) || (s2[0] == '-' && isdigit((unsigned char)s2[1]))) v2 = atoi(s2);
    else v2 = get_variable_value(s2, no_of_variables);

    return evaluate_condition(v1, op, v2);
}

void execute_c_minus_minus(char *code) {
    if (!code) return;
    char *p = code;
    while (*p) {
        p = skip_ws(p);
        if (!*p) break;

        if (strncmp(p, "if", 2) == 0 && (isspace((unsigned char)p[2]) || p[2] == '(')) {
            p += 2;
            char* cond = get_parentheses(&p);
            char* true_block = get_block(&p);

            char* false_block = NULL;
            char* next_p = skip_ws(p);
            if (strncmp(next_p, "else", 4) == 0 && (isspace((unsigned char)next_p[4]) || next_p[4] == '{')) {
                p = next_p + 4;
                false_block = get_block(&p);
            }

            if (eval_cond_str(cond)) {
                execute_c_minus_minus(true_block);
            } else if (false_block) {
                execute_c_minus_minus(false_block);
            }

            free(cond);
            free(true_block);
            if (false_block) free(false_block);

            p = skip_ws(p);
            if (*p == ';') p++;
        } else if (strncmp(p, "while", 5) == 0 && (isspace((unsigned char)p[5]) || p[5] == '(')) {
            p += 5;
            char* cond_str = get_parentheses(&p);
            char* body = get_block(&p);

            while (eval_cond_str(cond_str)) {
                char* body_exec = strdup(body);
                execute_c_minus_minus(body_exec);
                free(body_exec);
            }
            free(cond_str);
            free(body);

            p = skip_ws(p);
            if (*p == ';') p++;
        } else if (*p == '}') {
            p++;
        } else {
            char* start = p;
            while (*p && *p != ';') p++;
            size_t len = p - start;
            char* stmt = malloc(len + 1);
            memcpy(stmt, start, len);
            stmt[len] = '\0';
            if (*p == ';') p++;

            split(stmt);
            free(stmt);
        }
    }
}

void split(char* token) {
    if (!token) return;

    int capacity = 10;
    char** chararr = malloc(capacity * sizeof(char*));
    int count = 0;

    char* p = token;
    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        char* start = p;
        while (*p && !isspace((unsigned char)*p)) p++;
        int len = p - start;
        if (count >= capacity) {
            capacity *= 2;
            chararr = realloc(chararr, capacity * sizeof(char*));
        }
        chararr[count] = malloc(len + 1);
        memcpy(chararr[count], start, len);
        chararr[count][len] = '\0';
        count++;
    }

    if (count == 0) {
        free(chararr);
        return;
    }

    int spaces = count - 1;

    if (strcmp(chararr[0], "int") == 0) {
        create_variable(spaces, chararr, no_of_variables);
        no_of_variables++;
        bool has_equal = false;
        for (int i = 0; i < count; i++) {
            if (strcmp(chararr[i], "=") == 0) {
                has_equal = true;
                break;
            }
        }
        if (has_equal) {
            assign_variable(spaces, no_of_variables, token, chararr);
        }
    } else if (strcmp(chararr[0], "print") == 0) {
        print_variable(token, no_of_variables, chararr, spaces);
    } else {
        bool has_equal = false;
        for (int i = 0; i < count; i++) {
            if (strcmp(chararr[i], "=") == 0) {
                has_equal = true;
                break;
            }
        }
        if (has_equal) {
            assign_variable(spaces, no_of_variables, token, chararr);
        }
    }

    for (int i = 0; i < count; i++) free(chararr[i]);
    free(chararr);
}

int whitespaces(char* token) {
    int count = 0;
    bool in_ws = true;
    for (int i = 0; token[i]; i++) {
        if (isspace((unsigned char)token[i])) {
            if (!in_ws) count++;
            in_ws = true;
        } else {
            in_ws = false;
        }
    }
    return count;
}

int* arr_whitespaces(int spaces, char* token) {
    int* arr = malloc((spaces + 2) * sizeof(int));
    if (!arr) return NULL;
    int count = 0;
    bool in_ws = true;
    arr[count++] = -1;
    for (int i = 0; token[i]; i++) {
        if (isspace((unsigned char)token[i])) {
            if (!in_ws) arr[count++] = i;
            in_ws = true;
        } else {
            in_ws = false;
        }
    }
    arr[count] = strlen(token);
    return arr;
}

char* slicing(int start, int stop, char* token) {
    int len = stop - start;
    if (len < 0) len = 0;
    char* res = malloc(len + 1);
    if (!res) return NULL;
    memcpy(res, token + start, len);
    res[len] = '\0';
    return res;
}
