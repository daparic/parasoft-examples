#include "config.h"

#define MAX_PATH_LENGTH 4096
#define INPUT_BUFFER_SIZE 1024

#define STR_(X) #X
#define STR(X) STR_(X)

typedef unsigned int year_t;
typedef unsigned char month_t;
typedef unsigned char day_t;

#define PRIYEAR "u"
#define PRIMONTH "u"
#define PRIDAY "u"
#define SCNYEAR "u"
#define SCNMONTH "hhu"
#define SCNDAY "hhu"

typedef unsigned char hour_t;
typedef unsigned char minute_t;

#define PRIHOUR "u"
#define PRIMINUTE "u"
#define SCNHOUR "hhu"
#define SCNMINUTE "hhu"

typedef struct datetime_t {
    year_t year;
    month_t month;
    day_t day;
    hour_t hour;
    minute_t minute;
} datetime_t;

int valid_datetime(const datetime_t * const dt)
{
    /* Check date. */
    if (!((dt->year >= 1900u) && (dt->year <= 9999u))) {
        fprintf(stderr, "invalid year: %u\n", dt->year);
        return 0;
    }

    if (!((dt->month >= 1u) && (dt->month <= 12u))) {
        fprintf(stderr, "invalid month: %u\n", dt->month);
        return 0;
    }

    if (!((((dt->day >= 1u) && (dt->day <= 31u)) &&
           ((dt->month == 1u) || (dt->month == 3u) || (dt->month == 5u) || (dt->month == 7u) ||
            (dt->month == 8u) || (dt->month == 10u) || (dt->month == 12u))) ||
          (((dt->day >= 1u) && (dt->day <= 30u)) &&
           ((dt->month == 4u) || (dt->month == 6u) || (dt->month == 9u) || (dt->month == 11u))) ||
          (((dt->day >= 1u) && (dt->day <= 28u)) &&
           (dt->month == 2u)) ||
          ((dt->day == 29u) &&
           (dt->month == 2u) &&
           (((dt->year % 400u) == 0u) || (((dt->year % 4u) == 0u) && ((dt->year % 100u) != 0u)))))) {
        fprintf(stderr, "invalid date: %" PRIYEAR "-%" PRIMONTH "-%" PRIDAY "\n",
                dt->year, dt->month, dt->day);
        return 0;
    }

    /* Check time. */
    if (!(((dt->hour >= 0u) && (dt->hour <= 23u)) &&
          ((dt->minute >= 0u) && (dt->minute <= 59u)))) {
        fprintf(stderr, "invalid time: %" PRIHOUR ":%" PRIMINUTE "\n", dt->hour, dt->minute);
        return 0;
    }

    return 1;
}

datetime_t parse_datetime(const char * const datetime_str)
{
    datetime_t dt;

    if ((sscanf(datetime_str, "%" SCNYEAR "-%" SCNMONTH "-%" SCNDAY " %" SCNHOUR ":%" SCNMINUTE,
                &(dt.year), &(dt.month), &(dt.day), &(dt.hour), &(dt.minute))) != 5) {
        perror("sscanf parse datetime failed");
        fprintf(stderr, "input: '%s'\n", datetime_str);
        exit(-1);
    }

    return dt;
}

#define DATETIME_STR_SIZE 16

void string_datetime(const datetime_t * const datetime, char * const buffer)
{
    if(sprintf(buffer,
               "%04" PRIYEAR "-%02" PRIMONTH "-%02" PRIDAY " %02" PRIHOUR ":%02" PRIMINUTE,
               datetime->year,
               datetime->month,
               datetime->day,
               datetime->hour,
               datetime->minute) != DATETIME_STR_SIZE) {
        fputs("sprintf datetime failed\n", stderr);
        exit(-1);
    }
}

typedef unsigned int roomnum_t;

#define PRIROOMNUM "u"
#define SCNROOMNUM "u"

int valid_roomnum(const roomnum_t rn)
{
    return (rn >= 1) && (rn <= 9999);
}

size_t get_nlines(const char * const filename)
{
    FILE * f = NULL;
    size_t nlines = 0;
    int c = 0;

    if ((f = fopen(filename, "r")) == NULL) {
        perror("fopen failed");
        exit(-1);
    }

    for (c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n') {
            ++nlines;
        }
    }

    if (fclose(f) != 0) {
        perror("fclose failed");
        exit(-1);
    }

    return nlines;
}

#define MAX_NAME_SIZE 1024
#define DATETIME_STR_SIZE 16
#define MAX_ROOMNUM_STR_SIZE 4

typedef struct guest_reservation {
    char first_name[MAX_NAME_SIZE + 1];
    char last_name[MAX_NAME_SIZE + 1];
    datetime_t check_in;
    datetime_t check_out;
    roomnum_t room_num;
    
} guest_reservation_t;

#define SCNNAME "[A-Za-z. ]"
#define SCNDATE "[-0-9: ]"

void parse_entry(guest_reservation_t * const entry, const char * const entry_str)
{
    char check_in_str[DATETIME_STR_SIZE + 1] = {0};
    char check_out_str[DATETIME_STR_SIZE + 1] = {0};

    if (sscanf(entry_str,
               "%" STR(MAX_NAME_SIZE) SCNNAME ",%" STR(MAX_NAME_SIZE) SCNNAME ",%" STR(DATETIME_STR_SIZE) SCNDATE ",%" STR(DATETIME_STR_SIZE) SCNDATE ",%" STR(MAX_ROOMNUM_STR_SIZE) SCNROOMNUM,
               entry->last_name, entry->first_name, check_in_str, check_out_str, &(entry->room_num)) != 5) {
        fputs("scanf failed\n", stderr);
        exit(-1);
    }

    entry->check_in = parse_datetime(check_in_str);
    entry->check_out = parse_datetime(check_out_str);
}

int valid_reservation(const guest_reservation_t * const reservation)
{
    return (strlen(reservation->first_name) > 0) &&
        (strlen(reservation->last_name) > 0) &&
        valid_datetime(&(reservation->check_in)) &&
        valid_datetime(&(reservation->check_out)) &&
        valid_roomnum(reservation->room_num);
}

guest_reservation_t * read_hotel_book(const char * const filename,
                                       size_t * const num_reservations)
{
    guest_reservation_t * reservations = NULL;
    guest_reservation_t * curr_reservation = NULL;
    FILE * f = NULL;
    char * line = NULL;
    size_t len = 0;
    unsigned int read = 0;

    /* One entry per line, number of lines is number of entries. */
    *num_reservations = get_nlines(filename);

    /* Allocate memory. */
    reservations = malloc(sizeof(guest_reservation_t) * *num_reservations);

    if (reservations == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    memset(reservations, 0, sizeof(guest_reservation_t) * *num_reservations);

    /* Read file line by line. */

    curr_reservation = reservations;

    if ((f = fopen(filename, "r")) == NULL) {
        perror("fopen failed");
        exit(-1);
    }

    while ((read = getline(&line, &len, f)) != -1) {
        parse_entry(curr_reservation, line);

        if (!(valid_reservation(curr_reservation))) {
            fprintf(stderr, "invalid entry: '%s'\n", line);

            if (fclose(f) != 0) {
                perror("fclose failed");
                exit(-1);
            }

            exit(-1);
        }

        ++curr_reservation;
    }

    free(line);

    if (fclose(f) != 0) {
        perror("fclose failed");
        exit(-1);
    }

    return reservations;
}

int _get_line(const char * const prompt,
             char * const buffer,
             const int buffer_size)
{
    int input_size = 0;
    /* Display prompt if applicable. */
    if (prompt != NULL) {
        fputs(prompt, stdout);
        fflush(stdout);
    }

    /* Read user input. */
    if (fgets(buffer, buffer_size, stdin) == NULL) {
        return EOF;
    }

    input_size = strlen(buffer);

    if (input_size == 0) {
        return EOF;
    }

    /* If input was too long finish reading till newline and return error. */
    if (buffer[input_size - 1] != '\n') {
        int ch = '\0';
        int extra = 0;

        while (((ch = getchar()) != '\n') && (ch != EOF)) {
            extra = 1;
        }

        return extra ? INT_MIN : input_size;
    }

    buffer[input_size - 1] = '\0';
    return input_size;
}

char * repeat_char(const char c,
                   char * const buffer,
                   const size_t buffer_size,
                   const size_t count)
{
    char * curr = NULL;
    int i = 0;

    if (count >= buffer_size) {
        fputs("repeat character out of bounds\n", stderr);
        exit(-1);
    }

    memset(buffer, 0, buffer_size);

    curr = buffer;

    for (i = 0; i < count; ++i) {
        *curr = c;
        ++curr;
    }

    return buffer;
}

typedef enum name_lookup_type {
    FIRST = 0,
    LAST = 1,
} name_lookup_type;

void perform_lookup(guest_reservation_t * reservations,
                                     const size_t num_reservations,
                                     const name_lookup_type lookup_type)
{
    size_t nmatches = 0;
    guest_reservation_t * curr_reservation = NULL;
    int input_size = 0;
    char input_buffer[INPUT_BUFFER_SIZE] = {0};
    int i = 0;

    /* Ask user for name. */
    printf("> Lookup by %s name: ", (lookup_type == FIRST) ? "first" : "last");

    /*
     * === CWE 120 :: Classic Buffer Overflow
     *
     * -- DESCRIPTION --
     *   This vulnerability occurs when a program copies a buffer from an external
     *   source to an internal buffer without verifying it's size.
     *
     *   In this case the program is asking the user for a first or last name to
     *   search through reservations to find a match. The program copies the
     *   user's input into the input_buffer without checking to make sure it will
     *   fit beforehand. If the user provides a large input then the program will
     *   overflow and overwrite other stack values.
     *
     *   This kind of vulnerability could be exploited to cause a crash, buffer
     *   overflow, data leak, denial of service, or remote code execution.
     *
     * -- KNOWN CASES --
     *   CVE-2019-1010218 : Cherokee Webserver
     *   CVE-2017-12718   : Smiths Medical Medfusion 4000 Wireless Syringe Infusion Pump
     *   CVE-2011-5320    : glibc
     *   CVE-2006-0451    : Fedora Directory Server (LDAP)
     *   CVE-2003-0595    : TeraScript WiTango
     *   CVE-2002-1337    : sendmail
     *
     * -- DETECTED BY --
     *   C++Test : SECURITY-14
     *   Insure  : WRITE_OVERFLOW
     */
    scanf("%s", input_buffer);
    input_size = strlen(input_buffer);

    /* Gobble new line. */
    if (input_buffer[input_size - 1] != '\n') {
        int ch = '\0';
        while (((ch = getchar()) != '\n') && (ch != EOF)) {}
    }


    /* Search reservations and print matches. */
    curr_reservation = reservations;

    for (i = 0; i < num_reservations; ++i) {
        if (strncasecmp(input_buffer,
                        (lookup_type == FIRST) ? curr_reservation->first_name : curr_reservation->last_name,
                        input_size) == 0) {
            char checkin_str[DATETIME_STR_SIZE + 1] = {0};
            char checkout_str[DATETIME_STR_SIZE + 1] = {0};
            string_datetime(&(curr_reservation->check_in), checkin_str);
            string_datetime(&(curr_reservation->check_out), checkout_str);

            puts("\n* Found Match!");
            printf("  - Guest :: %s, %s\n", curr_reservation->last_name, curr_reservation->first_name);
            printf("  - Stay :: %s -> %s\n", checkin_str, checkout_str);
            printf("  - Room :: %" PRIROOMNUM "\n", curr_reservation->room_num);

            ++nmatches;
        }

        ++curr_reservation;
    }

    /* Display no matches if none found. */
    if (nmatches == 0) {
        puts("\nNo matches.");
    }
}

void tui_lookup(guest_reservation_t * reservations,
                const size_t num_reservations)
{
    char input_buffer[INPUT_BUFFER_SIZE] = {0};

    while (1) {
        int input_size = 0;

        puts("\n---- Hotel Booking Reservation :: Reservation Lookup ----");
        puts("  A: Lookup by last name.");
        puts("  B: Lookup by first name.");
        puts("  Q: Return to main menu.");

        input_size = _get_line("> Selection: ", input_buffer, INPUT_BUFFER_SIZE);

        if (input_size == EOF) {
            fputs("received EOF\n", stderr);
            break;
        } else if (input_size == INT_MIN) {
            fputs("user input too large\n", stderr);
            continue;
        }

        switch(*input_buffer) {
            case 'a':
            case 'A':
                perform_lookup(reservations, num_reservations, LAST);
                break;
            case 'b':
            case 'B':
                perform_lookup(reservations, num_reservations, FIRST);
                break;
            case 'q':
            case 'Q':
                return;
                break;
            default:
                puts("!!! Invalid option !!!!");
                break;
        }
    }
}

void tui_printall(guest_reservation_t * reservations,
                  const size_t num_reservations)
{
    guest_reservation_t * curr_reservation = reservations;
    int i = 0;

    /* Find longest first and last names for future formatting. */
    int longest_last_name = 9;
    int longest_first_name = 10;
    char linesep[INPUT_BUFFER_SIZE] = {0};
    char checkin_str[DATETIME_STR_SIZE + 1] = {0};
    char checkout_str[DATETIME_STR_SIZE + 1] = {0};

    for (i = 0; i < num_reservations; ++i) {
        size_t last_name_len = strlen(curr_reservation->last_name);
        size_t first_name_len = strlen(curr_reservation->first_name);

        if (last_name_len > longest_last_name) {
            longest_last_name = last_name_len;
        }

        if (first_name_len > longest_first_name) {
            longest_first_name = first_name_len;
        }

        ++curr_reservation;
    }

    /* Print table header. */
    printf("\n %-*s | %-*s | %-*s | %-*s | %s \n",
           longest_last_name, "Last Name",
           longest_first_name, "First Name",
           DATETIME_STR_SIZE, "Check-In",
           DATETIME_STR_SIZE, "Check-Out",
           "Room #");

    /* Print separator. */

    printf("-%s-+", repeat_char('-', linesep, INPUT_BUFFER_SIZE, longest_last_name));
    printf("-%s-+", repeat_char('-', linesep, INPUT_BUFFER_SIZE, longest_first_name));
    printf("-%s-+-%s-+",
           repeat_char('-', linesep, INPUT_BUFFER_SIZE, DATETIME_STR_SIZE), linesep);
    printf("-%s-\n", repeat_char('-', linesep, INPUT_BUFFER_SIZE, 6));

    /* Print all reservations. */
    curr_reservation = reservations;

    for (i = 0; i < num_reservations; ++i) {
        string_datetime(&(curr_reservation->check_in), checkin_str);
        string_datetime(&(curr_reservation->check_out), checkout_str);

        printf(" %*s | %*s | %*s | %*s | %" PRIROOMNUM " \n",
               longest_last_name, curr_reservation->last_name,
               longest_first_name, curr_reservation->first_name,
               DATETIME_STR_SIZE, checkin_str,
               DATETIME_STR_SIZE, checkout_str,
               curr_reservation->room_num);

        ++curr_reservation;
    }
}

void tui(const char * const filename)
{
    size_t num_reservations = 0;
    guest_reservation_t * reservations = read_hotel_book(filename, &num_reservations);

    int input_size = 0;
    char input_buffer[INPUT_BUFFER_SIZE] = {0};

    while (1) {
        puts("\n================================================");
        puts("==== Hotel Booking Reservation :: Main Menu ====");
        puts("  A: Print all reservations.");
        puts("  B: Lookup reservation.");
        puts("  Q: Quit.");

        input_size = _get_line("> Selection: ", input_buffer, INPUT_BUFFER_SIZE);

        if (input_size == EOF) {
            fputs("received EOF\n", stderr);
            break;
        } else if (input_size == INT_MIN) {
            fputs("user input too large\n", stderr);
            continue;
        }

        switch (*input_buffer) {
            case 'a':
            case 'A':
                tui_printall(reservations, num_reservations);
                break;
            case 'b':
            case 'B':
                tui_lookup(reservations, num_reservations);
                break;
            case 'q':
            case 'Q':
                break;
            default:
                puts("!!! Invalid option !!!!");
                break;
        }

        if ((*input_buffer == 'q') || (*input_buffer == 'Q')) {
            break;
        }
    }

    free(reservations);
}

void usage()
{
    fputs("Usage: demo120 [-h] -f file\n", stderr);
    fputs("   Hotel reservation booking system.\n", stderr);
    exit(-1);
}

int main(int argc, char* argv[])
{
    int opt = 0;
    char filename[MAX_PATH_LENGTH + 1] = {0};

    while ((opt = getopt(argc, argv, "f:h")) != -1) {
        switch (opt) {
            case 'f':
                strncpy(filename, optarg, MAX_PATH_LENGTH);
                break;
            case 'h':
                usage();
                break;
            case '?':
            default:
                exit(-1);
                break;
        }
    }

    if (strlen(filename) == 0) {
        fputs("missing argument: -f file\n", stderr);
        usage();
    }

    tui(filename);

    return 0;
}
