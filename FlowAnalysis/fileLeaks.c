#include "dbutil.h"
#include <stdio.h>

/*
 * Here are some examples to demonstrate how Flow Analysis 
 * detects leaks of file descriptors.
 */

void printFile(FILE* file)
{
    char c;
    printf("Reading file contents:\n\n");
    while(1) {
        c = fgetc(file);
        if (c!=EOF) {
            printf("%c", c);  
        } else {
            break;
        }
    }
}

int displayFileContent()
{
    FILE *file;
    file = fopen("myfile.txt", "r"); 
    if (file==NULL) {
        printf("Error: file can not be opened.\n");
        return 1;
    }
    printf("File successfully opened.\n\n");
    printFile(file);
    /* Uncomment the call to fclose() below to repair the code. */ 
    /* fclose(file); */
    return 0;
}

int displayContentOfSeveralFiles()
{
    int ARRSIZE = 3;
    static char *names[] = {
        "file1.txt",
        "file2.txt",
        "file3.txt",
    };
    int i;
    FILE *file = NULL;
    for (i = 0; i < ARRSIZE; ++i) {
        file = fopen(names[i], "r"); 
        if(file==NULL) {
            printf("Error: file can not be opened.\n");
            return 1;
        }
        printFile(file);
    }
    /* Put fclose() into the cycle to repair the code. */ 
    fclose(file);
    return 0;
}

