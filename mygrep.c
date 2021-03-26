#include <stdio.h>
#include <stdlib.h>
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
	[EXACT_MATCH]    = "-x",
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

void strtolow(char *str)
{
	for (char *c = str; *c; c++) {
		*c = tolower(*c);
	}
}

void print_output(char *str, struct s_args *args, bool prev_line_printed, int line_cnt, bool first_match, bool match, int byte_offset)
{
	bool *option = args->option;
	char delim = match ? ':' : '-';

	if (option[AFTER_CONTEXT] && !first_match && !prev_line_printed) {
		printf("--\n");
	}

	if (option[LINE_NUMBER]) {
		printf("%d%c", line_cnt, delim);
	}

	if (option[BYTE_OFFSET]) {
		printf("%d%c", byte_offset, delim);
	}

	printf("%s\n", str);
}

int is_pattern_at_place(char *place, char *pattern, bool ignore_case)
{
	if (!*pattern) {
		return true;
	}

	if (!*place) {
		return false;
	}

	if ((*place == *pattern) || (ignore_case && (tolower(*place) == tolower(*pattern)))) {
		return is_pattern_at_place(place + 1, pattern + 1, ignore_case);
	}

	return false;
}


int is_pattern_in_line(char *line, struct s_args *args)
{
	bool *option = args->option;
	char *pattern = args->pattern;
	bool match = false;

	while (!match && *line) {

		match = is_pattern_at_place(line, pattern, option[IGNORE_CASE]);

		if (option[EXACT_MATCH]) {
			match &= (strlen(line) == strlen(pattern));
			break;
		}

		line++;
	}

	if (option[INVERT_MATCH]) {
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
	char *line = NULL;
	int context = 0;
	bool match = false;
	bool prev_line_printed = false;
	int line_cnt = 0;
	int match_cnt = 0;
	int byte_cnt = 0;
	bool *option = args->option;

	while (1) {
		if (getline(&line, &n, args->fptr) < 0) {
			status = errno;
			break;
		}
		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = 0;
		}
		line_cnt++;

		prev_line_printed = match || context;
		match = is_pattern_in_line(line, args);
		context = is_context_line(match, context, args);

		if ((match || context) && !option[COUNT]) {
			print_output(line, args, prev_line_printed, line_cnt, !match_cnt, match, byte_cnt);
		}

		byte_cnt += strlen(line) + 1;
		match_cnt += match;

		free(line);
		line = NULL;
	}

	if (option[COUNT]) {
		printf("%d\n", match_cnt);
	}

	return status;
}

FILE *get_input_stream(char *path)
{
	FILE *fptr = fopen(path, "r");
	if (!fptr) {
		if (errno != ENOENT) {
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

	grep->fptr = get_input_stream(argv[1]);
	if (!grep->fptr) {
		return errno;
	}

	parse_options(argc, argv, grep);
	grep->pattern = argv[argc - 1];

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