#include <stdio.h>
#include <stdlib.h>
#include "dbutil.h"

/*
 * Here are some examples to demonstrate how Flow Analysis detects situations 
 * where there is a danger of a null pointer to be dereferenced.
 */
 
void storePersonToFile(Person *person, FILE* file)
{
    if (file!=NULL) {
        fprintf(file, "Id: %d Name: %s References:%s\n", 
        person->personalId, person->name, person->reference);
    }
    /* Avoid passing null to fclose! */
    fclose(file);
}

void copyPersonToFile(int id, FILE* file)
{
    Person* p = retrievePersonFromDB(id);
    storePersonToFile(p, file);
    free(p);
}

Person* createPerson(int id, char* name, char* ref)
{
    Person* p = malloc(sizeof(Person));
    p->name = name;
    p->personalId = id;
    p->reference = ref;
    return p;
}

void init()
{
    char c;
    Person* p = createPerson(7, "Alice", NULL);
    c = *p->reference;
    free(p);
}

