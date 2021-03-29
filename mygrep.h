
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum e_args
{
    AFTER_CONTEXT,
    EXTENDED_REGEX,
    BYTE_OFFSET,
    COUNT,
    IGNORE_CASE,
    LINE_NUMBER,
    INVERT_MATCH,
    EXACT_MATCH,
    OPTION_MAX
};

enum e_status
{
    STATUS_OK,
    STATUS_ERROR,
};

struct s_args
{
    FILE *fptr;
    int num_context_lines;
    char *regex;
    char *pattern;
    bool options[OPTION_MAX];
};


#define BIT(pos)              (1 << (pos))
#define SET_BIT(bitmap, pos)  ((bitmap) |= BIT(pos))
#define TEST_BIT(bitmap, pos) !!((bitmap) & BIT(pos))