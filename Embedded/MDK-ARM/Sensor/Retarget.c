#include <stdio.h>
#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

extern int SendChar (int ch);

struct __FILE { int handle; };

FILE __stdout;

int fputc (int ch, FILE *f) {
 return (SendChar(ch)); 
}

int ferror (FILE *f) {
  return EOF;
}

void _ttywrch (int ch) {
 SendChar(ch); 
}

void _sys_exit (int return_code) {
 for (;;); 
}

#if PARASOFT_CPPTEST
int fflush(FILE * fd) {
  return 0;
}
#endif
