#include "comp_ctype.h"

int ctype_isalpha(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}

int ctype_isdigit(char c) {
    return (c >= '0' && c <= '9') ? 1 : 0;
}

int ctype_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r') ? 1 : 0;
}
