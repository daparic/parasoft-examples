#include <limits.h>

#include "config.h"

#define MAX_PATH_LENGTH 4096
#define MAX_LONG_NAME 64
#define MAX_SHORT_NAME 16
#define MAX_QUANTITY_STR 20
#define INPUT_BUFFER_SIZE 1024

typedef struct item_amount {
    char long_name[MAX_LONG_NAME];
    char short_name[MAX_SHORT_NAME];
    size_t quantity;
} item_amount;

typedef struct recipe {
    char long_name[MAX_LONG_NAME];
    char short_name[MAX_SHORT_NAME];
    item_amount * ingredients;
    size_t num_ingredients;
} recipe;

unsigned int _get_line(const char * const prompt,
             char * const buffer,
             const size_t buffer_size)
{
    size_t input_size = 0;

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

size_t count_char(const char * const str, const char c)
{
    size_t count = 0u;
    int i = 0;

    for (i = 0; i < strlen(str); ++i) {
        if (str[i] == c) {
            ++count;
        }
    }

    return count;
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

char * repeat_char(const char c,
                   char * const buffer,
                   const size_t buffer_size,
                   const size_t count )
{
    int i = 0;
    if (count >= buffer_size) {
        fputs("repeat character out of bounds\n", stderr);
        exit(-1);
    }

    memset(buffer, 0, buffer_size);

    for (i = 0; i < count; ++i) {
        buffer[i] = c;
    }

    return buffer;
}

void print_stock(item_amount * items, const size_t num_items)
{
    /* Find longest short and long names for future formatting. */
    char quantity_str[MAX_QUANTITY_STR + 1];
    int longest_short_name = 10;
    int longest_long_name = 9;
    int longest_quantity = 8;
    size_t i = 0;
    char linesep[MAX_LONG_NAME + 1] = {0};

    for (i = 0; i < num_items; ++i) {
        item_amount * curr_item = &items[i];

        /* Get lengths of names for the current item. */
        size_t short_name_len = strlen(curr_item->short_name);
        size_t long_name_len = strlen(curr_item->long_name);

        /* Get the length of the string of the quantity. */
        int quantity_len = sprintf(quantity_str, "%zu", curr_item->quantity);

        if (quantity_len < 0) {
            perror("sprintf failed");
            exit(-1);
        }

        if (quantity_len + 1 > sizeof(quantity_str)) {
            fputs("sprintf buffer overflow\n", stderr);
            exit(-1);
        }

        /* Update longest values if appropriate. */
        if (short_name_len > longest_short_name) {
            longest_short_name = short_name_len;
        }

        if (long_name_len > longest_long_name) {
            longest_long_name = long_name_len;
        }

        if (quantity_len > longest_quantity) {
            longest_quantity = quantity_len;
        }
    }

    /* Print table header. */
    printf("\n %-*s | %-*s | %-*s \n",
           longest_long_name, "Long Name",
           longest_short_name, "Short Name",
           longest_quantity, "Quantity");

    /* Print separator. */

    printf("-%s-+", repeat_char('-', linesep, MAX_LONG_NAME + 1, longest_long_name));
    printf("-%s-+", repeat_char('-', linesep, MAX_LONG_NAME + 1, longest_short_name));
    printf("-%s-\n", repeat_char('-', linesep, MAX_LONG_NAME + 1, longest_quantity));

    /* Print stack. */
    for (i = 0; i < num_items; ++i) {
        item_amount * curr_item = &items[i];

        printf(" %*s | %*s | %zu \n",
               longest_long_name, curr_item->long_name,
               longest_short_name, curr_item->short_name,
               curr_item->quantity);
    }
}

item_amount * parse_stock(const char * const filename, size_t * num_items)
{
    item_amount * items = NULL;
    FILE * f = NULL;
    char * line = NULL;
    size_t len = 0;
    unsigned int nread = 0;
    item_amount * curr_item = 0;

    *num_items = get_nlines(filename);

    items = malloc(sizeof(item_amount) * *num_items);

    if (items == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    memset(items, 0, sizeof(item_amount) * *num_items);

    /* Read file line by line. */

    if ((f = fopen(filename, "r")) == NULL) {
        perror("fopen failed");
        exit(-1);
    }

    curr_item = items;

    while ((nread = getline(&line, &len, f)) != -1) {
        char * token = strtok(line, ",");
        size_t token_index = 0;

        while (token != NULL) {
            switch(token_index) {
                case 0:
                    strncpy(curr_item->long_name, token, MAX_LONG_NAME);
                    break;
                case 1:
                    strncpy(curr_item->short_name, token, MAX_SHORT_NAME);
                    break;
                case 2:
                    if ((sscanf(token, "%zu", &curr_item->quantity)) != 1) {
                        fputs("sscanf failed", stderr);
                        exit(-1);
                    }
                    break;
                default:
                    fputs("error parsing stock\n", stderr);
                    exit(-1);
            }

            token = strtok(NULL, ",");
            ++token_index;
        }

        ++curr_item;
    }

    free(line);

    if (fclose(f) != 0) {
        perror("fclose failed");
        exit(-1);
    }

    return items;
}

void print_recipes(recipe * recipes,
                   const size_t num_recipes,
                   item_amount * items,
                   const size_t num_items)
{
    size_t recipe_index = 0;
    size_t ingredient_index = 0;
    size_t item_index = 0;

    for (recipe_index = 0; recipe_index < num_recipes; ++recipe_index) {
        recipe * curr_recipe = &recipes[recipe_index];

        printf("* %s (%s)\n", curr_recipe->long_name, curr_recipe->short_name);

        for (ingredient_index = 0; ingredient_index < curr_recipe->num_ingredients; ++ingredient_index) {
            item_amount * curr_ingredient = &(curr_recipe->ingredients[ingredient_index]);
            item_amount * matching_item = NULL;

            for (item_index = 0; item_index < num_items; ++item_index) {
                item_amount * curr_item = &items[item_index];

                if (strcmp(curr_item->short_name, curr_ingredient->short_name) == 0) {
                    matching_item = curr_item;
                    break;
                }
            }

            if (matching_item == NULL) {
                fprintf(stderr, "could not find ingredient: '%s'\n", curr_ingredient->short_name);
                exit(-1);
            }

            printf("  - %zu %s\n", curr_ingredient->quantity, matching_item->long_name);
        }
    }
}

recipe * parse_recipes(const char * const filename, size_t * const num_recipes)
{
    recipe * recipes = NULL;
    FILE * f = NULL;
    char * line = NULL;
    size_t len = 0;
    unsigned int nread = 0;
    recipe * curr_recipe = NULL;

    *num_recipes = get_nlines(filename);

    recipes = malloc(sizeof(recipe) * *num_recipes);

    if (recipes == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    memset(recipes, 0, sizeof(recipe) * *num_recipes);

    /* Read file line by line. */

    if ((f = fopen(filename, "r")) == NULL) {
        perror("fopen failed");
        exit(-1);
    }

    curr_recipe = recipes;

    while ((nread = getline(&line, &len, f)) != -1) {
        char * token = NULL;
        size_t token_index = 0;
        size_t ingredients_index = 0;

        curr_recipe->num_ingredients = (count_char(line, ',') - 1) / 2;
        curr_recipe->ingredients = malloc(sizeof(item_amount) * curr_recipe->num_ingredients);

        token = strtok(line, ",");

        while (token != NULL) {
            switch(token_index) {
                case 0:
                    strncpy(curr_recipe->long_name, token, MAX_LONG_NAME);
                    break;
                case 1:
                    strncpy(curr_recipe->short_name, token, MAX_SHORT_NAME);
                    break;
                default:
                    /* Even means ingredient short name. *
                     * Odd means ingredient quantity. */
                    if ((token_index % 2) == 0) {
                        strncpy(curr_recipe->ingredients[ingredients_index].short_name,
                                token,
                                MAX_SHORT_NAME);
                    } else {
                        if ((sscanf(token,
                                   "%zu",
                                    &(curr_recipe->ingredients[ingredients_index].quantity))) != 1) {
                            fputs("sscanf failed\n", stderr);
                            exit(-1);
                        }
                        ++ingredients_index;
                    }
            }

            token = strtok(NULL, ",");
            ++token_index;
        }

        ++curr_recipe;
    }

    if (fclose(f) != 0) {
        perror("fclose failed");
        exit(-1);
    }

    free(line);

    return recipes;
}

void serve_drink(item_amount * items,
                 const size_t num_items,
                 recipe * recipes,
                 const size_t num_recipes)
{
    char input_buffer[INPUT_BUFFER_SIZE] = {0};
    /*
     * === CWE 121 :: Stack-based Buffer Overflow
     *
     * -- DESCRIPTION --
     *   This vulnerability occurs when a program overflows a buffer on the
     *   stack. This is often caused by naively copying buffers without
     *   considering their sizes.
     *
     *   In this case the developer calls the utility function _get_line() which
     *   gets a line of input from the user from standard input terminated by a
     *   newline or EOF. The _get_line() function is designed to safely get the
     *   input using the fgets() function and even gracefully handles extremely
     *   long inputs. However, the developer accidentally specified the wrong
     *   buffer size which cases a stack buffer overflow.
     *
     *   This kind of vulnerability could be exploited to cause a crash, buffer
     *   overflow, denial of service, or remote code execution.
     *
     * -- KNOWN CASES --
     *   CVE-2019-7714  : Green Hills INTEGRITY RTOS
     *   CVE-2018-12010 : Google Android
     *   CVE-2018-4249  : Apple macOS, iOS, tvOS
     *   CVE-2017-17863 : Linux kernel
     *   CVE-2012-1189  : The Open Raceing Car Simulator (TORCS)
     *   CVE-2008-3024  : QNX Momentics RTOS
     *
     * -- DETECTED BY --
     *   C++Test : BD-PB-OVERFWR
     *   Insure  : WRITE_OVERFLOW
     */
    unsigned int input_size = _get_line("> Drink short name: ", input_buffer, MAX_PATH_LENGTH);
    recipe * selected_recipe = NULL;
    size_t recipe_index;
    size_t ingredient_index;
    size_t item_index;

    if (input_size == EOF) {
        fputs("received EOF\n", stderr);
        return;
    } else if (input_size == INT_MIN) {
        fputs("user input too large\n", stderr);
        return;
    }

    /* Search for selected drink recipe. */

    for (recipe_index = 0; recipe_index < num_recipes; ++recipe_index) {
        recipe * curr_recipe = &recipes[recipe_index];

        if (strcmp(input_buffer, curr_recipe->short_name) == 0) {
            selected_recipe = curr_recipe;
            break;
        }
    }

    if (selected_recipe == NULL) {
        puts("!!! Invalid drink name !!!");
        return;
    }

    /* Remove ingredients from inventory. */
    for (ingredient_index = 0; ingredient_index < selected_recipe->num_ingredients; ++ingredient_index) {
        item_amount * curr_ingredient = &(selected_recipe->ingredients[ingredient_index]);

        /* Search for ingredient in stock. */
        item_amount * matching_invetory = NULL;

        for (item_index = 0; item_index < num_items; ++item_index) {
            item_amount * curr_item = &items[item_index];

            if (strcmp(curr_ingredient->short_name, curr_item->short_name) == 0) {
                matching_invetory = curr_item;
                break;
            }
        }

        if (matching_invetory == NULL) {
            printf("missing recipe ingredient in stock: '%s'\n", curr_ingredient->short_name);
            puts("partial ingredients discarded");
            return;
        }

        /* Take the required amount from stock. */
        if (matching_invetory->quantity < curr_ingredient->quantity) {
            printf("not enough ingredient %s (%s): requires %zu, have %zu\n",
                   matching_invetory->long_name,
                   matching_invetory->short_name,
                   curr_ingredient->quantity,
                   matching_invetory->quantity);
            puts("partial ingredients discarded");
            return;
        }

        matching_invetory->quantity -= curr_ingredient->quantity;
    }

    printf("Served a %s (%s).\n", selected_recipe->long_name, selected_recipe->short_name);
}

void tui(const char * const stock_filename, const char * const recipes_filename)
{
    size_t num_items = 0;
    item_amount * items = parse_stock(stock_filename, &num_items);
    size_t num_recipes = 0;
    recipe * recipes = parse_recipes(recipes_filename, &num_recipes);
    int recipe_index = 0;

    char input_buffer[INPUT_BUFFER_SIZE] = {0};

    while (1) {
        unsigned int input_size;

        puts("\n=======================================");
        puts("==== Bartending Buddy :: Main Menu ====");
        puts("  0: Display current inventory.");
        puts("  1: Display menu.");
        puts("  2: Serve a drink.");
        puts("  Q: Quit.");

        input_size = _get_line("> Selection: ", input_buffer, INPUT_BUFFER_SIZE);

        if (input_size == EOF) {
            fputs("received EOF\n", stderr);
            break;
        } else if (input_size == INT_MIN) {
            fputs("user input too large\n", stderr);
            continue;
        }

        switch (*input_buffer - '0') {
            case 0:
                print_stock(items, num_items);
                break;
            case 1:
                print_recipes(recipes, num_recipes, items, num_items);
                break;
            case 2:
                serve_drink(items, num_items, recipes, num_recipes);
                break;
            case 'q' - '0':
            case 'Q' - '0':
                break;
            default:
                puts("!!! Invalid option !!!!");
                break;
        }

        if ((*input_buffer == 'q') || (*input_buffer == 'Q')) {
            break;
        }
    }

    /* Cleanup memory. */
    free(items);

    for (recipe_index = 0; recipe_index < num_recipes; ++recipe_index) {
        free(recipes[recipe_index].ingredients);
    }

    free(recipes);
}

void usage()
{
    fputs("Usage: demo121 [-h] -s stock_file -r recipes_file\n", stderr);
    fputs("   Bartending Software Suite\n", stderr);
    exit(-1);
}

int main(int argc, char ** argv)
{
    int opt = 0;
    char stock_filename[MAX_PATH_LENGTH + 1] = {0};
    char recipes_filename[MAX_PATH_LENGTH + 1] = {0};

    while ((opt = getopt(argc, argv, "s:r:h")) != -1) {
        switch (opt) {
            case 's':
                strncpy(stock_filename, optarg, MAX_PATH_LENGTH);
                break;
            case 'r':
                strncpy(recipes_filename, optarg, MAX_PATH_LENGTH);
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

    tui(stock_filename, recipes_filename);

    return 0;
}
