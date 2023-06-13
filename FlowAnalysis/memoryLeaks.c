#include <stdlib.h>
#include <stdio.h>
#include "dbutil.h"

void storePerson(Person *p)
{
    /* stores a person data in a database */
}

void printPersonInfo(FILE* file)
{
    char *c;
    char buf[100];
    int id = 0;
    while(1) {
        c = fgets(buf, 100, file);
        if (c!=NULL) {
            Person* p = malloc(sizeof(Person));
            if (p == 0) exit(1);
            p->personalId = ++id;
            p->name = buf;
            storePerson(p);
       
            /* Uncomment the call to free(p) to avoid leaks of memory allocated for structure. */
            /* free(p); */
     } else {
         break;
     }
   }
}

void simpleLeakInCycle()
{
    int count = 0;
    int LOOPS = 100;
    int MAXSIZE = 100;
    char *pointer = NULL;

    for(count=0; count < LOOPS; count++) {
        pointer = (char *)malloc(sizeof(char) * MAXSIZE);
    }
   
    /* Repair: put the call to free() inside the cycle. */
    free(pointer);
}

