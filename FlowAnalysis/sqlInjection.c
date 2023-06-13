#include "sql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1000

const char* requestBeginning = "SELECT user_id, user_class, rights FROM users WHERE user_name = '";
const char* requestPassword = "' and password = '";
const char* requestEnding = "'";

char* extractUsername(char* s)
{
	/* Some implementation */
	return s;
}

char* extractPassword(char* s)
{
	/* Some implementation */
	return s;
}

void handleRequest(SQLHSTMT statementHandle, FILE *file)
{
    char parametersString[BUF_SIZE];
    char request[BUF_SIZE];

    fread(parametersString, 1, BUF_SIZE, file);
    strcpy(request, requestBeginning);
    strcat(request, extractUsername(parametersString));
    strcat(request, requestPassword);
    strcat(request, extractPassword(parametersString));
    strcat(request, requestEnding);
    SQLExecDirect(statementHandle, (SQLCHAR *)request, strlen(request));
}

