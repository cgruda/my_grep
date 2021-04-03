#include "match.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

bool is_char_end_of_pattern(char c)
{
	return (c == '\0') || (c == ')') || (c == '|');
}

bool is_char_match(char c1, char c2, bool ignore_case)
{
	return (c1 == c2) || (ignore_case && (tolower(c1) == tolower(c2)));
}

bool is_char_in_range(char c, char *range, bool ignore_case)
{
	char start, end;
	sscanf(range, "[%c-%c]", &start, &end); // FIXME: this could also be wrong if include '\'
	return ((c >= start) && (c <= end)) ||
		(ignore_case && ((tolower(c) >= tolower(start)) && (tolower(c) <= tolower(end))));
}

bool is_or_match_here(char **place, char *pattern, bool ignore_case)
{
	char *delim = strstr(pattern, "|"); // FIXME: this isnt safe!!!
	char *end = strstr(pattern, ")");
	bool match_a = is_match_here(*place, pattern + 1, ignore_case, false);
	bool match_b = is_match_here(*place, delim + 1, ignore_case, false);
	*place += match_a ? (delim - pattern - 2) : (end - delim - 2);
	return match_a || match_b;
}

bool is_match_here(char *text, char *pattern, bool ignore_case, bool exact_match)
{
	bool match = false;
	bool end_of_pattern = is_char_end_of_pattern(pattern[0]);

	if (!text[0]) {
		return end_of_pattern;
	} else if (end_of_pattern) {
		return !exact_match;
	}

	switch (pattern[0]) {
	case '.':
		match = true;
		break;
	case '[':
		match = is_char_in_range(text[0], pattern, ignore_case);
		pattern = strstr(pattern, "]");
		break;
	case '(':
		match = is_or_match_here(&text, pattern, ignore_case);
		pattern = strstr(pattern, ")");
		break;
	case '\\':
		pattern++;
	default:
		match = is_char_match(text[0], pattern[0], ignore_case);
	}

	if (match) {
		return is_match_here(text + 1, pattern + 1, ignore_case, exact_match);
	} else {
		return false;
	}
}

bool is_match_in_line(char *text, char *pattern, bool ignore_case, bool exact_match, bool invert_match)
{
	bool match = false;

	while (!match && text[0]) {
		match = is_match_here(text, pattern, ignore_case, exact_match);
		if (exact_match) {
			break;
		}
		text++;
	}

	return (invert_match ? !match : match);
}

int is_context_line(bool match, int num_context_lines, int context_lines_left)
{
	if (match) {
		return num_context_lines + 1;
	}

	if (context_lines_left > 0) {
		return context_lines_left - 1;
	}

	return 0;
}