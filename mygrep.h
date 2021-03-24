
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum e_args
{
    //
    ARG_A,
    ARG_E,
    //
    ARG_b,
    ARG_c,
    ARG_i,
    ARG_n,
    ARG_v,
    ARG_x,
    ARG_MAX
};

enum e_status
{
    STATUS_OK,
    STATUS_ERROR,
};

struct s_args
{
    FILE *fp;
    int flags;
    int  arg_A_num;
    char arg_E_regx;
};


#define BIT(pos)              (1 << (pos))
#define SET_BIT(bitmap, pos)  ((bitmap) |= BIT(pos))
#define TEST_BIT(bitmap, pos) !!((bitmap) & BIT(pos))