#include "match.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MIN_ARGC 2

enum grep_options
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

struct grep_env {
	FILE *fptr;
	char *pattern;
	bool options[OPTION_MAX];
	int context_line_cnt;
	int line_num;
	int byte_cnt;
	int match_cnt;
};

const char *opt_str[OPTION_MAX] = {
	[AFTER_CONTEXT]  = "-A",
	[REGEX]          = "-E",
	[BYTE_OFFSET]    = "-b",
	[COUNT]          = "-c",
	[IGNORE_CASE]    = "-i",
	[LINE_NUMBER]    = "-n",
	[INVERT_MATCH]   = "-v",
	[EXACT_MATCH]    = "-x",
};

FILE *get_input_stream(char *path);
void parse_options(int argc, char **argv, struct grep_env *grep);
int grep_init(int argc, char **argv, struct grep_env *grep);
char *read_line(FILE *stream);
void print_line(char *line, struct grep_env *grep, bool prev_line_printed, bool match);

int main(int argc, char **argv)
{
	struct grep_env grep = {0};
	int context = 0;
	bool match = false;
	if (grep_init(argc, argv, &grep) < 0) {
		return EXIT_FAILURE;
	}

	while (1) {
		char *line = read_line(grep.fptr);
		if (!line) {
			break;
		}
		bool prev_line_printed = match || context;
		match = is_match_in_line(line, grep.pattern, grep.options[IGNORE_CASE], grep.options[EXACT_MATCH], grep.options[INVERT_MATCH]);
		context = is_context_line(match, context, grep.context_line_cnt);
		if ((match || context) && !grep.options[COUNT]) {
			print_line(line, &grep, prev_line_printed, match);
		}
		grep.byte_cnt += strlen(line) + 1;
		grep.match_cnt += match;
		grep.line_num++;
		free(line);
		line = NULL;
	}

	if (grep.options[COUNT]) {
		printf("%d\n", grep.match_cnt);
	}

	fclose(grep.fptr);
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

void parse_options(int argc, char **argv, struct grep_env *grep)
{
	for (int i = 1; i < argc - 1; i++) {
		if (!strcmp(argv[i], opt_str[AFTER_CONTEXT])) {
			grep->context_line_cnt = atoi(argv[i + 1]);
			i++;
			continue;
		}

		for (int opt_idx = REGEX; opt_idx < OPTION_MAX; opt_idx++) {
			if (!strcmp(argv[i], opt_str[opt_idx])) {
				grep->options[opt_idx] = true;
				break;
			}
		}
	}
}

int grep_init(int argc, char **argv, struct grep_env *grep)
{
	if (argc < MIN_ARGC) {
		printf("Usage: ./my_grep [OPTIONS] PATTERN [FILE]\n");
		return -1;
	}

	grep->fptr = get_input_stream(argv[argc - 1]);
	if (!grep->fptr) {
		return -1;
	}

	int pattern_arg_idx = argc - 1 - (errno != ENOENT);
	grep->pattern = argv[pattern_arg_idx];
	parse_options(argc, argv, grep);
	grep->line_num = 1;

	return 0;
}

char *read_line(FILE *stream)
{
	char *line = NULL;
	size_t n = 0;

	if (getline(&line, &n, stream) < 0) {
		free(line);
		return NULL;
	}

	if (line[strlen(line) - 1] == '\n') {
		line[strlen(line) - 1] = 0;
	}

	return line;
}

void print_line(char *line, struct grep_env *grep, bool prev_line_printed, bool match)
{
	bool *option = grep->options;
	char delim = match ? ':' : '-';

	if (option[AFTER_CONTEXT] && (grep->match_cnt > 0) && !prev_line_printed) {
		printf("--\n");
	}

	if (option[LINE_NUMBER]) {
		printf("%d%c", grep->line_num, delim);
	}

	if (option[BYTE_OFFSET]) {
		printf("%d%c", grep->byte_cnt, delim);
	}

	printf("%s\n", line);
}
