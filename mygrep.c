#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include "mygrep.h"

// =========== debug ==========
#define DBG_MODE        0
#define DBG(...)        printf(__VA_ARGS__)

enum error {
	ESUCCESS,
	EINPUT,
	EMAX
};


const char *opt_str[OPTION_MAX] =
{
	[AFTER_CONTEXT]  = "-A",
	[EXTENDED_REGEX] = "-E",
	[BYTE_OFFSET]    = "-b",
	[COUNT]          = "-c",
	[IGNORE_CASE]    = "-i",
	[LINE_NUMBER]    = "-n",
	[INVERT_MATCH]   = "-v",
	[LINE_REGEXP]    = "-x",
};

#define max(a, b) ((a) > (b) ? (a) : (b))

void usage_print()
{
	printf("usage: ./mygrep [file] [-A num] [-b] [-c] [-i] [-n] [-v] [-x] [-E regex] <pattern>\n");
}

#define MIN_ARGC 2

int parse_options(int argc, char **argv, struct s_args *args)
{
	for (int i = 1; i < argc - 1; i++) {
		if (!strcmp(argv[i], opt_str[AFTER_CONTEXT])) {
			args->option[AFTER_CONTEXT] = true;
			args->num_context_lines = atoi(argv[i + 1]);
			i++;
			continue;
		}

		if (!strcmp(argv[i], opt_str[EXTENDED_REGEX])) {
			args->option[EXTENDED_REGEX] = true;
			args->regex = argv[i + 1];
			i++;
			continue;
		}

		for (int opt_idx = BYTE_OFFSET; opt_idx < OPTION_MAX; opt_idx++) {
			if (!strcmp(argv[i], opt_str[opt_idx])) {
				args->option[opt_idx] = true;
			}
		}
	}

}

int parse_args(int argc, char **argv, struct s_args *args)
{
	

	args->fptr = fopen(argv[1], "r");
	if (!args->fptr) {
		if (errno != ENOENT) {
			return errno;
		}
		args->fptr = stdin;
	}

	args->str = argv[argc - 1];

	return ESUCCESS;
}

void strtolow(char *p)
{
	while (*p) {
		*p = tolower(*p++);
	}
}

void print_output(char *str, struct s_args *args, bool prev_line_printed, int line_cnt, bool first_match, bool match, int byte_offset)
{
	bool *option = args->option;
	char delim = match ? ':' : '-';

	if (option[COUNT]) {
		return;
	}

	if (option[AFTER_CONTEXT] && !first_match && !prev_line_printed) {
		printf("--\n");
	}

	if (option[LINE_NUMBER]) {
		printf("%d%c", line_cnt, delim);
	}

	if (option[BYTE_OFFSET]) {
		printf("%d%c", byte_offset, delim);
	}

	printf("%s", str);

	if (str[strlen(str) - 1] != '\n') { // FIXME: this is ugly
		printf("\n");
	}
}

int is_match_line(char *line, struct s_args *args)
{
	bool match = false;

	match = strstr(line, args->str) != NULL;

	if (args->option[INVERT_MATCH]) {
		match = !match;
	}

	return match;
}

int is_context_line(bool match, int context, struct s_args *args)
{
	if (context > 0) {
		context--;
	}
	
	if (match && args->option[AFTER_CONTEXT]) {
		context = args->num_context_lines + 1;
	}

	return context;
}

int mygrep(struct s_args *args)
{
	int status = ESUCCESS;
	size_t n = 0;
	char *lineptr = NULL;
	int context = 0;
	bool match = false;
	bool prev_line_printed = false;
	int line_cnt = 0;
	int match_cnt = 0;
	int byte_cnt = 0;

	while (1) {

		// FIXME: how do we exit if in stdin mode?

		if (getline(&lineptr, &n, args->fptr) < 0) {
			status = errno;
			break;
		}
		line_cnt++;

		prev_line_printed = match || context;
		match = is_match_line(lineptr, args);
		context = is_context_line(match, context, args);
		if (match || context) {
			print_output(lineptr, args, prev_line_printed, line_cnt, !match_cnt, match, byte_cnt);
		}

		byte_cnt += strlen(lineptr);
		match_cnt += match;

		free(lineptr);
		lineptr = NULL;
	}

	if (args->option[COUNT]) {
		printf("%d\n", match_cnt);
	}

	return status;
}

FILE *get_input_stream(char *path)
{
	FILE *fptr = fopen(path, "r");
	if (!fptr) {
		if (errno != ENOENT) {
			DBG("%d\n", __LINE__);
			return NULL;
		}
		fptr = stdin;
	}
	return fptr;
}

int grep_init(int argc, char **argv, struct s_args *grep)
{
	if (argc < MIN_ARGC) {
		usage_print();
		return EINPUT;
	}

	if (!(grep->fptr = get_input_stream(argv[1]))) {
		return errno;
	}

	parse_options(argc, argv, grep);
	grep->str = argv[argc - 1];

	return ESUCCESS;
}

int main(int argc, char **argv)
{
	struct s_args grep = {0};
	int status = ESUCCESS;

	status = grep_init(argc, argv, &grep);
	if (status != ESUCCESS) {
		return status;
	}

	status = mygrep(&grep);
	fclose(grep.fptr); // FIXME: check with stdin
	return status;
}