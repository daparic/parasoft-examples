
#ifndef _timer_h_
#define _timer_h_

#include <time.h>


/* timere structure */
struct timer_record
{
    time_t starttime;
    time_t endtime;
    unsigned channel;
};

/* init/uninit routines for the timer */
void init_timer();
void uninit_timer();

/* adds a timer, queries user for info, return ERROR_CODE on failure */
int add_timer();
    
/* adds a timer record */
void add_timer_record(struct timer_record*);

/* delete a timer */
void delete_timer_record(int);

/* get string for a single timer */
void format_timer_record(int, char*);

/* display list of all timers */
void list_timers();

#endif /* _timer_h_ */

