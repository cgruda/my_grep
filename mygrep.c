#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include "mygrep.h"

#define DBG(...)        printf(__VA_ARGS__)
#define MIN_ARGC 2

enum error {
	ESUCCESS,
	EINPUT,
	EMAX
};

const char *opt_str[OPTION_MAX] =
{
	[AFTER_CONTEXT]  = "-A",
	[REGEX]          = "-E",
	[BYTE_OFFSET]    = "-b",
	[COUNT]          = "-c",
	[IGNORE_CASE]    = "-i",
	[LINE_NUMBER]    = "-n",
	[INVERT_MATCH]   = "-v",
	[EXACT_MATCH]    = "-x",
};

// init
int grep_init(int argc, char **argv, struct grep_env *grep);
void print_usage();
FILE *get_input_stream(char *path);
void parse_options(int argc, char **argv, struct grep_env *grep);

// search
bool is_char_end_of_pattern(char c);
// bool is_char_regex(char c) ;
bool is_char_in_range(char c, char *range, bool ignore_case);
bool is_char_match(char c1, char c2, bool ignore_case);
bool is_match_here(char *place, char *pattern, bool ignore_case, bool exact_match);
// bool is_regex_match(char **place, char **pattern, bool ignore_case);
bool is_match_in_text(char *text, char *pattern, bool *options);
int is_context_line(bool match, int context, int context_line_cnt);


char *read_line(FILE *stream);
void print_line(char *line, struct grep_env *grep, bool prev_line_printed, bool match);




void print_usage()
{
	printf("Usage: ./mygrep [OPTIONS] PATTERN [FILE]\n");
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

int grep_init(int argc, char **argv, struct grep_env *grep)
{
	if (argc < MIN_ARGC) {
		print_usage();
		return EINPUT;
	}

	grep->fptr = get_input_stream(argv[argc - 1]);
	if (!grep->fptr) {
		return errno;
	}

	int pattern_arg_idx = argc - 1 - (errno != ENOENT);
	grep->pattern = argv[pattern_arg_idx];
	parse_options(argc, argv, grep);
	grep->line_num = 1;

	return ESUCCESS;
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

bool is_char_end_of_pattern(char c)
{
	return (c == '\0') || (c == ')') || (c == '|');
}

bool is_char_in_range(char c, char *range, bool ignore_case)
{
	char start, end;
	sscanf(range, "[%c-%c]", &start, &end);
	return ((c >= start) && (c <= end)) ||
		((tolower(c) >= tolower(start)) && (tolower(c) <= tolower(end)) && ignore_case);
}

// bool is_char_regex(char c) 
// {
// 	return (c == '\\') || (c == '.') || (c == '[') || (c == '(');
// }

bool is_char_match(char c1, char c2, bool ignore_case)
{
	return (c1 == c2) || (ignore_case && (tolower(c1) == tolower(c2)));
}

// bool is_regex_match(char **place, char **pattern, bool ignore_case)
// {
// 	bool match_a = false, match_b = false;
// 	bool match = false;

// 	switch (*pattern[0]) {
// 	case '.':
// 		match = true;
// 		break;
// 	case '[':
// 		match = is_char_in_range(*place[0], *pattern, ignore_case);
// 		*pattern = strstr(*pattern, "]");
// 		break;
// 	case '(':
// 		match_a = is_match_here(*place, *pattern + 1, ignore_case, true, false);
// 		match_b = is_match_here(*place, strstr(*pattern, "|") + 1, ignore_case, true, false);
// 		match = match_a || match_b;
// 		*place += match_a ? (strstr(*pattern, "|") - *pattern - 2) : (strstr(*pattern, ")") - strstr(*pattern, "|") - 2);
// 		*pattern = strstr(*pattern, ")");
// 		break;
// 	case '\\':
// 		*pattern++;
// 		// fall through
// 	default:
// 		match = is_char_match(*place, *pattern, ignore_case);
// 	}

// 	return match;
// }

bool is_match_here(char *place, char *pattern, bool ignore_case, bool exact_match)
{
	bool match = false, match_a = false, match_b = false;
	bool end_of_pattern = is_char_end_of_pattern(pattern[0]);
	bool end_of_line = !place[0];

	if (end_of_line) {
		return end_of_pattern;
	} else if (end_of_pattern) {
		return !exact_match;
	}

	switch (pattern[0]) {
	case '.':
		match = true;
		break;
	case '[':
		match = is_char_in_range(place[0], pattern, ignore_case);
		pattern = strstr(pattern, "]");
		break;
	case '(':
		match_a = is_match_here(place, pattern + 1, ignore_case, false);
		match_b = is_match_here(place, strstr(pattern, "|") + 1, ignore_case, false);
		match = match_a || match_b;
		place += match_a ? (strstr(pattern, "|") - pattern - 2) : (strstr(pattern, ")") - strstr(pattern, "|") - 2);
		pattern = strstr(pattern, ")");
		break;
	case '\\':
		pattern++;
	default:
		match = is_char_match(place[0], pattern[0], ignore_case);
	}

	if (match) {
		return is_match_here(place + 1, pattern + 1, ignore_case, exact_match);
	} else {
		return false;
	}
}

bool is_match_in_text(char *text, char *pattern, bool *options)
{
	bool match = false;

	while (!match && text[0]) {
		match = is_match_here(text, pattern, options[IGNORE_CASE], options[EXACT_MATCH]);
		if (options[EXACT_MATCH]) {
			break;
		}
		text++;
	}

	return (options[INVERT_MATCH] ? !match : match);
}

int is_context_line(bool match, int context_lines_left, int context_line_cnt)
{
	if (match) {
		return context_line_cnt + 1;
	}

	if (context_lines_left > 0) {
		return context_lines_left - 1;
	}

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

int main(int argc, char **argv)
{
	struct grep_env grep = {0};
	char *line = NULL;
	int context = 0;
	bool match = false, prev_line_printed = false;

	int status = grep_init(argc, argv, &grep);
	if (status != ESUCCESS) {
		return status;
	}

	do {
		line = read_line(grep.fptr);
		if (!line) {
			break;
		}

		prev_line_printed = match || context;
		match = is_match_in_text(line, grep.pattern, grep.options);
		context = is_context_line(match, context, grep.context_line_cnt);
		if ((match || context) && !grep.options[COUNT]) {
			print_line(line, &grep, prev_line_printed, match);
		}

		grep.byte_cnt += strlen(line) + 1;
		grep.match_cnt += match;
		grep.line_num++;
		free(line);
		line = NULL;

	} while (true);

	if (grep.options[COUNT]) {
		printf("%d\n", grep.match_cnt);
	}

	fclose(grep.fptr);
	return errno;
}
