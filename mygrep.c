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

// =========== error ==========
#define INPUT_ERR       1
#define MEM_ERR         2

// ========== other =========
#define MAX_BUFFER  100
#define MAXLEN          100
#define MAX_LINE_LEN    100

// args dict
const char *args_str[ARG_MAX] =
{
    // coupled options
    [ARG_A] = "-A",
    [ARG_E] = "-E",
    // standalone options
    [ARG_b] = "-b",
    [ARG_c] = "-c",
    [ARG_i] = "-i",
    [ARG_n] = "-n",
    [ARG_v] = "-v",
    [ARG_x] = "-x",
};

int init(int argc, char **argv, struct s_args *args)
{
	if (argc < 3)
	return 0;

	memset(args, 0, sizeof(*args));

	/* open file, if no file use stdin */
	args->fp = fopen(argv[1], "r");
	if (!args->fp) {
		if (errno != ENOENT)
			return 0;
		args->fp = stdin;
	}

	/* parse commands */
	for (int i = 1; i < argc; i++) {

		if (strcmp(argv[i], args_str[ARG_A])) {
			args->arg_A_num = strtol(argv[i+1], NULL, 10);
			SET_BIT(args->flags, ARG_A);
			i++;
			continue;
		}

		if (strcmp(argv[i], args_str[ARG_E])) {
			args->arg_E_regx = argv[i+1];
			SET_BIT(args->flags, ARG_E);
			i++;
			continue;
		}

		for (int j = ARG_b; j < ARG_MAX; j++)
			if (strcmp(argv[i], args_str[j]))
				SET_BIT(args->flags, j);
	}

	return 1;
}

// turn string to lower case
void strtolow(char *p)
{
	while (*p)
		*p = tolower(*p++);
}

// remove eol char for string
void remove_eol(char *line)
{
	for (char *p = line; *p; ++p) {
		if(*p == '\n') {
			*p =  '\0';
			break;
		}
	}
}

void mygrep(FILE *fp, int *flags, char *str)
{
    char buffer[MAX_BUFFER];
    int line_num = 0;
    int force_print = 0;
    bool match = 0;
    bool new_line = 0;
    char *c;

    FILE *p_line_begin = NULL;

    while (true)
    {
	// init search
	match = false;
	memset(buffer, 0, MAX_BUFFER);

	// fill buffer
	getline(&buffer, MAX_BUFFER, fp);

	// rm newline
	if (c = strrchr(buffer, '\n'))
	{
	    new_line = true;
	    c = 0;
	}
	else
	{

	}
	
	

    }

    while(!feof(fptr))
    {
	found = false;
	
	// remove '\n' from end of line
	
	
	// lower upper case "-i"
	if(flags[ARG_i]) {
	    strtolow(line);
	    strtolow(str);
	}

	// "-x" strict search
	if (flags[ARG_x])
	    found = strcmp(line, str) ? False : True;
	else
	    found = strstr(line, str) ? True : False;
	    

	if (found) {

	    force_print = flags[ARG_A];

	    // print line number "-n" and "-c"
	    if (flags[ARG_n] || flags[ARG_c])
		printf("%d:", line_num);

	    // dont print actual line if "-c"
	    if (!flags[ARG_c])
		printf("\t%s",line);
	    
	    printf("\n");
	} 
	else {      // print after lines "-A"
	    
	    if (force_print && !flags[ARG_c]) {
		printf(">\t%s\n",line);
		--force_print;
	    }
	}    
	++line_num;
    
    }
}

int main(int argc, char *argv[])
{
    struct s_args args;

    init(argc, argv, &args);


    mygrep(fptr, flags, argv[argc - 2]);
    
    fclose(fptr);
    free(flags);

    return 1;
}