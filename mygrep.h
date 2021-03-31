
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum e_option
{
    AFTER_CONTEXT,
    REGEX,
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

struct grep_env
{
	FILE *fptr;
	char *pattern;
	bool options[OPTION_MAX];
	int context_line_cnt;
	
	bool is_match;
	bool is_prev_match;
	bool is_context;
	bool is_prev_context;
	bool priv_line_printed;

	int line_num;
	int byte_cnt;
	int match_cnt;
};
