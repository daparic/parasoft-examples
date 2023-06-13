
#ifndef _FLOWANALYSIS_SQL_H_
#define _FLOWANALYSIS_SQL_H_

#ifdef WIN32
#define SQL_API  __stdcall
#else
#define SQL_API
#endif

typedef unsigned char   SQLCHAR;
typedef long            SQLINTEGER;

#if defined(WIN32)
typedef void*           SQLHANDLE;
#else
typedef SQLINTEGER      SQLHANDLE;
#endif	/* defined(WIN32) */

typedef short           SQLSMALLINT;
typedef SQLSMALLINT     SQLRETURN;
typedef SQLHANDLE       SQLHSTMT;

SQLRETURN  SQL_API SQLExecDirect(SQLHSTMT StatementHandle,
           SQLCHAR *StatementText, SQLINTEGER TextLength);

#endif

