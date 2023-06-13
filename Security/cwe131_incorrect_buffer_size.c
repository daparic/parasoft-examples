#include "config.h"

#define MAX_PATH_LENGTH 4096

unsigned char * sanitize_html_entity_characters(const unsigned char * const content, const size_t content_size, size_t * const sanitized_size)
{
    size_t dst = 0;
    int i = 0;

    /*
     * === CWE 131 :: Incorrect Buffer Size ===
     *
     * -- DESCRIPTION --
     *   This vulnerability occurs when the size of a buffer is not calculated
     *   correctly which can lead to a buffer overflow.
     *
     *   In this case the program is trying to calculate the space required to
     *   sanitize the HTML entity characters. The assumption is that if every
     *   character has to be substituted then the largest size of substitution
     *   multiplied by the size of the original contents. Unfortunately in this
     *   case the largest size of substitution is not 5 but 6.
     *
     *   This type vulnerability could be exploited into a heap buffer overflow
     *   that when combined with other exploitation techniques could lead to
     *   crash, denial of service, or remote code execution.
     *
     * -- KNOWN CASES --
     *   CVE-2019-8375 : WebKit
     *   CVE-2019-3973 : Comodo Antivirus
     *   CVE-2018-8822 : Linux Kernel
     *   CVE-2018-3866 : Samsung SmartThings Hub Firmware
     *   CVE-2011-3048 : libpng
     *
     * -- DETECTED BY --
     *   Insure : HEAP_CORRUPT
     *            WRITE_BAD_INDEX
     */
    unsigned char * sanitized_content = malloc(content_size * 5);

    if (sanitized_content == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    for (i = 0; i < content_size; i++) {
        switch (content[i]) {
            case '<':
                sanitized_content[dst++] = '&';
                sanitized_content[dst++] = 'l';
                sanitized_content[dst++] = 't';
                sanitized_content[dst++] = ';';
                break;
            case '>':
                sanitized_content[dst++] = '&';
                sanitized_content[dst++] = 'g';
                sanitized_content[dst++] = 't';
                sanitized_content[dst++] = ';';
                break;
            case '&':
                sanitized_content[dst++] = '&';
                sanitized_content[dst++] = 'a';
                sanitized_content[dst++] = 'm';
                sanitized_content[dst++] = 'p';
                sanitized_content[dst++] = ';';
                break;
            case '"':
                sanitized_content[dst++] = '&';
                sanitized_content[dst++] = 'q';
                sanitized_content[dst++] = 'u';
                sanitized_content[dst++] = 'o';
                sanitized_content[dst++] = 't';
                sanitized_content[dst++] = ';';
                break;
#ifndef __HC12__                
            case '\'':
                sanitized_content[dst++] = '&';
                sanitized_content[dst++] = 'a';
                sanitized_content[dst++] = 'p';
                sanitized_content[dst++] = 'o';
                sanitized_content[dst++] = 's';
                sanitized_content[dst++] = ';';
                break;
#endif                
            default:
                sanitized_content[dst++] = content[i];
        }
    }

    *sanitized_size = dst;

    return sanitized_content;
}

unsigned char * read_file(const char * const filename, size_t * const bytesread)
{
    FILE * f = NULL;
    long fsize = 0;
    unsigned char * buffer = NULL;

    if ((f = fopen(filename, "r")) == NULL) {
        perror("fopen failed");
        exit(-1);
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        perror("fseek end failed");
        exit(-1);
    }

    if ((fsize = ftell(f)) == -1) {
        perror("ftell failed");
        exit(-1);
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        perror("fseek beginning failed");
        exit(-1);
    }

    buffer = malloc(fsize + 1);

    if (buffer == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    if (fread(buffer, 1, fsize, f) != fsize) {
        perror("fread failed");
        exit(-1);
    }

    if (fclose(f) != 0) {
        perror("fclose failed");
        exit(-1);
    }

    buffer[fsize] = 0;

    *bytesread = fsize + 1;

    return buffer;
}

void usage()
{
    fputs("Usage: demo131 [-h] -f file\n", stderr);
    fputs("    Sanitize HTML entity characters.\n", stderr);
    exit(-1);
}

int main(int argc, char* argv[])
{
    int opt = 0;
    char filename[MAX_PATH_LENGTH + 1] = {0};
    size_t content_size = 0;
    unsigned char * content = NULL;
    unsigned char * sanitized = NULL;
    size_t sanitized_size = 0;

    while ((opt = getopt(argc, argv, "f:h")) != -1) {
        switch (opt) {
            case 'f':
                strncpy(filename, optarg, MAX_PATH_LENGTH);
                break;
            case 'h':
                usage();
                break;
            case '?':
                exit(-1);
                break;
        }
    }

    if (strlen(filename) == 0) {
        fputs("missing argument: -f file\n", stderr);
        usage();
    }

    content = read_file(filename, &content_size);

    if ((content == NULL) || (content_size == 0)) {
        fputs("error reading file\n", stderr);
        exit(-1);
    }

    sanitized = sanitize_html_entity_characters(content, content_size, &sanitized_size);

    printf("%s\n", sanitized);

    free(content);
    free(sanitized);

    return 0;
}

