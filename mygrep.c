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

void parse_options(int argc, char **argv, struct s_args *args)
{
	bool *options = args->options;

	for (int i = 1; i < argc - 1; i++) {
		if (!strcmp(argv[i], opt_str[AFTER_CONTEXT])) {
			options[AFTER_CONTEXT] = true; // FIXME: may remove
			args->num_context_lines = atoi(argv[i + 1]);
			i++;
			continue;
		}

		if (!strcmp(argv[i], opt_str[EXTENDED_REGEX])) {
			options[EXTENDED_REGEX] = true;
			args->regex = argv[i + 1];
			i++;
			continue;
		}

		for (int opt_idx = BYTE_OFFSET; opt_idx < OPTION_MAX; opt_idx++) {
			if (!strcmp(argv[i], opt_str[opt_idx])) {
				options[opt_idx] = true;
				break;
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
	bool *option = args->options;
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


int is_pattern_in_line(char *line, char *pattern, bool *options)
{
	bool match = false;

	while (!match && *line) {
		match = is_pattern_at_place(line, pattern, options[IGNORE_CASE]);
		if (options[EXACT_MATCH]) {
			match &= (strlen(line) == strlen(pattern));
			break;
		}
		line++;
	}

	if (options[INVERT_MATCH]) {
		match = !match;
	}

	return match;
}

int is_context_line(bool match, int context, int num_context_lines)
{
	if (match) {
		return num_context_lines + 1;
	}

	if (context > 0) {
		return context - 1;
	}

	return 0;
}

char *read_line(FILE *stream)
{
	char *line = NULL;
	size_t n = 0;

	if (getline(&line, &n, stream) < 0) {
		return NULL;
	}

	// remove newline charachter
	if (line[strlen(line) - 1] == '\n') {
		line[strlen(line) - 1] = 0;
	}

	return line;
}

int mygrep(struct s_args *args)
{
	int line_cnt = 1;
	int match_cnt = 0;
	int byte_cnt = 0;

	char *line = NULL;
	int context = 0;

	bool match = false;
	bool prev_line_printed = false;
	bool *options = args->options;

	while (1) {
		if (!(line = read_line(args->fptr))) {
			break;
		}

		prev_line_printed = match || context;

		match = is_pattern_in_line(line, args->pattern, options);
		context = is_context_line(match, context, args->num_context_lines);
		if ((match || context) && !options[COUNT]) {
			print_output(line, args, prev_line_printed, line_cnt, !match_cnt, match, byte_cnt);
		}

		byte_cnt += strlen(line) + 1;
		match_cnt += match;
		line_cnt++;
		free(line);
		line = NULL;
	}

	if (options[COUNT]) {
		printf("%d\n", match_cnt);
	}

	return errno;
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

	grep->fptr = get_input_stream(argv[argc - 1]);
	if (!grep->fptr) {
		return errno;
	}

	int pattern_argc_idx = argc - 1 - (errno != ENOENT);
	grep->pattern = argv[pattern_argc_idx];
	parse_options(argc, argv, grep);

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