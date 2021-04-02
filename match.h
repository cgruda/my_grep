#ifndef _MATCH_H_
#define _MATCH_H_

#include <stdbool.h>

bool is_char_end_of_pattern(char c);
bool is_char_in_range(char c, char *range, bool ignore_case);
bool is_char_match(char c1, char c2, bool ignore_case);
bool is_match_here(char *place, char *pattern, bool ignore_case, bool exact_match);
bool is_match_in_line(char *text, char *pattern, bool ignore_case, bool exact_match, bool invert_match);
bool is_or_match_here(char **place, char *pattern, bool ignore_case);
int is_context_line(bool match, int context, int context_line_cnt);

#endif // _MATCH_H_