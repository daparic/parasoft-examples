#include "config.h"

#define MAGIC_MIN_LENGTH 12
#define MAX_PATH_LENGTH 4096

typedef enum image_format {
    UNKNOWN = 0,
    JPG = 1,
    PNG = 2,
    GIF = 3,
    TIF = 4,
    PSD = 5,
    PDF = 6,
    RAW = 7,
} image_format;

static const unsigned char magic_jpg0[] = "\xFF\xD8\xFF\xDB";
static const unsigned char magic_jpg1[] = "\xFF\xD8\xFF\xE0\x00\x10\x4A\x46\x49\x46\x00\x01";
static const unsigned char magic_jpg2[] = "\xFF\xD8\xFF\xEE";
static const unsigned char magic_jpg3_0[] = "\xFF\xD8\xFF\xE1";
static const unsigned char magic_jpg3_1[] = "\x45\x78\x69\x66\x00\x00";
static const unsigned char magic_png[] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A";
static const unsigned char magic_gif0[] = "\x47\x49\x46\x38\x37\x61";
static const unsigned char magic_gif1[] = "\x47\x49\x46\x38\x39\x61";
static const unsigned char magic_tif0[] = "\x49\x49\x2A\x00";
static const unsigned char magic_tif1[] = "\x4D\x4D\x00\x2A";
static const unsigned char magic_psd[] = "\x38\x42\x50\x53";
static const unsigned char magic_pdf[] = "\x25\x50\x44\x46\x2d";
static const unsigned char magic_raw[] = "\x49\x49\x2A\x00\x10\x00\x00\x00\x43\x52";

unsigned char * get_magic_bytes(const char * const filename)
{
    FILE * f = NULL;
    unsigned char * magic_bytes = NULL;

    if ((f = fopen(filename, "r")) == NULL) {
        perror("fopen failed");
        exit(-1);
    }

    if (fseek(f, MAGIC_MIN_LENGTH, SEEK_SET) != 0) {
        perror("fseek failed");
        fputs("file too small\n", stderr);
        exit(-1);
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        perror("fseek beginning failed");
        exit(-1);
    }

    magic_bytes = malloc(MAGIC_MIN_LENGTH + 1);

    if (magic_bytes == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    if (fread(magic_bytes, 1, MAGIC_MIN_LENGTH, f) != MAGIC_MIN_LENGTH) {
        perror("fread failed");
        exit(-1);
    }

    if (fclose(f) != 0) {
        perror("fclose failed");
        exit(-1);
    }

    magic_bytes[MAGIC_MIN_LENGTH] = 0;
    
    return magic_bytes;
}

image_format determine_image_format(const char * const filename)
{
    image_format imgfmt = UNKNOWN;
    unsigned char * magicbytes = get_magic_bytes(filename);

    /* JPG -- Joint Photographic Experts Group */
    if ((memcmp(magicbytes, magic_jpg0, sizeof(magic_jpg0) - 1) == 0) ||
        (memcmp(magicbytes, magic_jpg1, sizeof(magic_jpg1) - 1) == 0) ||
        (memcmp(magicbytes, magic_jpg2, sizeof(magic_jpg2) - 1) == 0) ||
        ((memcmp(magicbytes, magic_jpg3_0, sizeof(magic_jpg3_0) - 1) == 0) &&
         (memcmp(magicbytes + 5, magic_jpg3_1, sizeof(magic_jpg3_1) - 1) == 0))) {
        imgfmt = JPG;
    /* PNG -- Portable Network Graphics */
    } else if (memcmp(magicbytes, magic_png, sizeof(magic_png) - 1) == 0) {
        imgfmt = PNG;
    /* GIF -- Graphics Interchange Format */
    } else if ((memcmp(magicbytes, magic_gif0, sizeof(magic_gif0) - 1) == 0) ||
               (memcmp(magicbytes, magic_gif1, sizeof(magic_gif1) - 1) == 0)) {
        imgfmt = GIF;
    /* TIF -- Tagged Image File Format */
    } else if ((memcmp(magicbytes, magic_tif0, sizeof(magic_tif0) - 1) == 0) ||
               (memcmp(magicbytes, magic_tif1, sizeof(magic_tif1) - 1) == 0)) {
        imgfmt = TIF;
    /* PSD -- Photoshop Document */
    } else if (memcmp(magicbytes, magic_psd, sizeof(magic_psd) - 1) == 0) {
        imgfmt = PSD;
    /* PDF -- Portable Document Format */
    } else if (memcmp(magicbytes, magic_pdf, sizeof(magic_pdf) - 1) == 0) {
        imgfmt = PDF;
    /* RAW -- Raw Sensor Data */
    } else if (memcmp(magicbytes, magic_raw, sizeof(magic_raw) - 1) == 0) {
        imgfmt = RAW;
    }

    free(magicbytes);

    return imgfmt;
}

char * get_image_format_name(image_format imgfmt)
{
    switch (imgfmt) {
        case UNKNOWN:
            return "Unknown";
            break;
        case JPG:
            return "Joint Photographic Experts Group";
            break;
        case PNG:
            return "Portable Network Graphics";
            break;
        case GIF:
            return "Graphics Interchange Format";
            break;
        case TIF:
            return "Tagged Image File Format";
            break;
        case PSD:
            return "Photoshop Document";
            break;
        case PDF:
            return "Portable Document Format";
            break;
        case RAW:
            return "Raw Sensor Data";
            break;
        default:
            return "Error";
    }
}

void print_image_format(const char * const filename)
{
    size_t fn_len = 0;
    char * fn = NULL;
    image_format imgfmt;

    puts("--- Image Information ---");
    fputs( "  * User Input     : '", stdout);
    /*
     * === CWE 134 :: Uncontrolled Format String
     *
     * -- DESCRIPTION --
     *   This vulnerability occurs when a function accepts a format string as an
     *   argument and the format string originates form an external source.
     *
     *   In this case the program is trying to analyze the magic bytes of a file
     *   in order to determine the format of an image and print the results. The
     *   filename provided to the program is assumed to be a path to a
     *   file. However, the user can provide a format string allowing unintended
     *   functionality.
     *
     *   This kind of vulnerability could be exploited to cause a crash, buffer
     *   overflow, data leak, denial of service, or remote code execution.
     *
     * -- KNOWN CASES --
     *   CVE-2019-12297 : Motorola CX2 and M2 routers
     *   CVE-2019-7712  : Green Hills INTEGRITY RTOS
     *   CVE-2018-0202  : ClamAV clamscan
     *   CVE-2017-11262 : Adobe Acrobat
     *   CVE-2017-9212  : BMW 330i
     *
     * -- DETECTED BY --
     *   C++Test : SECURITY-36
     *   Insure  : BAD_FORMAT
     */
    printf(filename);
    puts("'");

    fn_len = strlen(filename);
    fn = malloc(fn_len + 1);
    strncpy(fn, filename, fn_len + 1);

    printf("  * Directory Name : '%s'\n", dirname(fn));
    printf("  * Base File Name : '%s'\n", basename(fn));

    imgfmt = determine_image_format(filename);

    printf("  * Image Format   : '%s'\n", get_image_format_name(imgfmt));

    free(fn);
}

void usage()
{
    fputs("Usage: demo134 [-h] -f file\n", stderr);
    fputs("    Detect image file format.\n", stderr);
    exit(-1);
}

int main(int argc, char *argv[])
{
    int opt;
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
                exit(-1);
                break;
        }
    }

    if (strlen(filename) == 0) {
        fputs("missing argument: -f file\n", stderr);
        usage();
    }

    print_image_format(filename);

    return 0;
}
