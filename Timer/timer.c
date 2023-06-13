
/*
 * Timer class, stores timer records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"


static struct timer_record* timer_records[BUF_SIZE];
const int max_records = BUF_SIZE;
static int curr_index = 0;

void init_timer()
{
    memset(timer_records, 0, sizeof(struct timer_record*) * BUF_SIZE); 
}

void uninit_timer()
{
    /* Remove all timer records */
    int i;

    for (i = 0; i < max_records; i++) {
        delete_timer_record(i);
    }
}

/*
 * Queries user for timer information
 */
struct timer_record* query_user()
{
    int start_h, start_m, end_h, end_m;
    struct timer_record* the_record;
    time_t timer;
    struct tm* tm_tmp;

    timer = time(NULL);
    tm_tmp = localtime(&timer);
    
    the_record = (struct timer_record*)malloc(sizeof(struct timer_record));
    memset(the_record, 0, sizeof(struct timer_record));
    
    /* starttime */
    print_string("Please enter the start hour [0-23] > ");
    tm_tmp->tm_hour = get_input_digit();
    print_string("Please enter the start minute [0-59] > ");
    tm_tmp->tm_min = get_input_digit();
    
    the_record->starttime = mktime(tm_tmp);

    /* endtime */
    print_string("\nPlease enter the end hour [0-23] > ");
    tm_tmp->tm_hour = get_input_digit();
    print_string("\nPlease enter the end minute [0-59] > ");
    tm_tmp->tm_min = get_input_digit();
    
    the_record->endtime = mktime(tm_tmp);

    /* channel */
    print_string("\nPlease enter the channel to record > ");
    the_record->channel = get_input_digit();

    return (the_record);
}

int add_timer()
{
    struct timer_record* record;

    record = query_user();

    if (record) {
        add_timer_record(record);
    } else {
        return ERROR_CODE;
    }
    return 0;
}

void add_timer_record(struct timer_record* tr)
{
#ifdef OUTPUT
    {
        char[50] buf;
        sprintf(buf, "Curr Index = %d\n", curr_index);
        _EB_SEND(buf)
    }
#endif
    if (curr_index < max_records) {
        timer_records[curr_index++] = tr;
    } else {
        print_string("\nAll timers used ... timer not added\n");
    }
}

/*
 * Removed record at idx
 * Moves all records past idx, up one slot
 */
void delete_timer_record(int idx)
{
    struct timer_record* tr = timer_records[idx];
    int i;
    
    /* fill in the holes */
    for (i = idx-1; i < curr_index; i++)
    {
        if (0 == timer_records[i]) {
            break;
        } else {
            timer_records[i] = timer_records[i+1];
        }
    }
    curr_index--;
    free(tr);
}

void format_timer_record(int idx, char* buf)
{
    char start[BUF_SIZE];
    char end[BUF_SIZE];
    
    struct timer_record* tr = timer_records[idx];

    strftime(start, BUF_SIZE, "%I:%M %p", localtime(&tr->starttime));
    strftime(end, BUF_SIZE, "%I:%M %p", localtime(&tr->endtime));

    if (tr) {
        sprintf(buf, "%d\t%s\t%s\t%d\n", idx+1, start, end, tr->channel);
    }
    
}

void list_timers()
{
    char buf[BUF_SIZE];
    int i;
    print_string("\n\nCurrent Set Timers");
    print_string("\nRecord#\tStart Time\tEnd Time\tChannel\n");
    for (i = 0; i < curr_index; i++)
    {
        format_timer_record(i, buf);
        print_string(buf);
    }
    print_string("\n\n");
}

