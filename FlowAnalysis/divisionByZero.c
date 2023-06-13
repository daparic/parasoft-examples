#include "dbutil.h"
#include <stdio.h>
#include <stdlib.h>

void displayPersonalInfo(Person *person)
{
    printf("Name: %s\n", person->name);
    printf("Salary: %d\n", person->salary);
    printf("Day's pay: %d\n", person->salary/person->daysWorkedInPassedMonth);
}

Person* createNewPerson(char* name, int salary)
{
    Person* p = malloc(sizeof(Person));
    if (p == 0) exit(1);
    p->name = name;
    p->salary = salary;
    p->daysWorkedInPassedMonth = 0;
    displayPersonalInfo(p);
    return p;
}

int calculateAverageSalary(int numberOfEmployees)
{
    int WAGE_FUND = 10000;
    return WAGE_FUND/numberOfEmployees;
}

void processStaff(Person* employees[], int sizeOfStaff)
{
    int i;
    for (i = 0; i < sizeOfStaff; ++i) {
        displayPersonalInfo(employees[i]);
    }
    printf("Average salary: %d\n", calculateAverageSalary(sizeOfStaff));
}

int processFile(FILE* file, Person* employees[])
{
    /** Consider we had an error reading the file causing zero number of records read. */
    return 0;
}

int main()
{
    FILE *file = fopen("staff.txt", "r"); 
    Person* employees[100];
    int numberOfEmployees;
    if (file==NULL) {
        printf("Error: file can not be opened.\n");
        return 1;
    }
    numberOfEmployees = processFile(file, employees);
    processStaff(employees, numberOfEmployees);
    fclose(file);
    return 0;
}

