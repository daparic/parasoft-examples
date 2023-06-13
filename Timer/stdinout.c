
/* 
 * Implements routines from inout.h for stdin/stdout
 */

#include "inout.h"
#include "consts.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Grabs input from stdin
 */
int get_input_digit()
{
    char c;
    int i;
    char buf[BUF_SIZE];

#ifdef STDINPUT
    /* define STDINPUT to be able to accept input from a console */
    for (i = 0; i < BUF_SIZE; i++) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            break;
        } else if (isdigit(c)) {
            buf[i] = c;
        }
    }
#else /* STDINPUT */

    /* to allow automated unit testing */
    static int index = 0;

    /* simulate user input */
    ++ index;
    index %= 10;
    buf[0] = (char)('0' + index);
    i = 1;

#endif /* STDINPUT */
    
    buf[i] = '\0';
    return (atoi(buf));
}

/*
 * Print string to stdout
 */
int print_string(char* str)
{
    return (printf(str));
}
