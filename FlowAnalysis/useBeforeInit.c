#include <stdio.h>
#include <stdlib.h>
#include "dbutil.h"

/*
 * Here are some examples to demonstrate how Flow Analysis detects situations 
 * when a variable is used prior to its explicit initialization.
 */

void storePersonToDB(Person *p)
{
    /* stores a person data in a database */
}
 
void writePersonToFile(Person *person, char* filename)
{
    FILE *file;
    int numberOfCharactersPrinted;
    file = fopen(filename, "w");
   
    if (file!=NULL) {
        numberOfCharactersPrinted = fprintf(file, "Id: %d Name: %s References:%s\n", 
        person->personalId, person->name, person->reference);
        fclose(file);
    }
  
    printf("File length: %d\n", numberOfCharactersPrinted);
}

void copyPersonFromFileToDB(char* filename)
{
    Person* p;
    FILE *file;
    file = fopen(filename, "r");
    if (file!=NULL) {
        char id[10];
        char name[100];
        char reference[1000];
        p = malloc(sizeof(Person));
        if (p == 0) exit(1);
        fgets(id, 10, file);
        p->personalId = atoi(id);
        fgets(name, 100, file);
        p->name = name;
        fgets(reference, 100, file);
        p->reference = reference;
   
        fclose(file);
    }
    storePersonToDB(p);
    free(p);
}

