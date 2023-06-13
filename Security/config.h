#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (defined(_MSC_VER) || defined(__MINGW32__)) && !defined(__ghs)
 #define popen _popen
 #define pclose _pclose
#else
 FILE *popen (const char *__command, const char *__modes);
 int pclose (FILE *__stream);
#endif  /* (defined(_MSC_VER) || defined(__MINGW32__)) && !defined(__ghs) */

#ifndef NAME_MAX
 #define NAME_MAX  255
#endif /* !NAME_MAX */

extern char *optarg;  

/* dirent.h */ 
struct dirent
{
    char d_name[NAME_MAX + 1];   /* file name (null-terminated) */
};

typedef struct dirent DIR;

DIR             *opendir(const char *name);
int             closedir(DIR *dir);
void            rewinddir(DIR *dir);
struct dirent   *readdir(DIR *dir);

 /* netinet/in.h */
#define INADDR_ANY   0  

struct in_addr {
    unsigned int s_addr;     
};   

struct sockaddr_in {  
    unsigned char sin_len;
    unsigned char sin_family;
    unsigned short sin_port; 
    struct in_addr sin_addr;  
    char sin_zero[8];
};

typedef size_t socklen_t;

int getopt(int argc, char * const argv[], const char *optstring); 

int strncasecmp(const char *s1, const char *s2, size_t n);

/* sys/socket.h */
struct sockaddr {  
    unsigned char     sa_len; 
    unsigned char     sa_family;  
    char              sa_data[14];  
};

#define SOL_SOCKET   0xffff  
#define SOCK_STREAM  1  
#define AF_INET      2   

#define SO_REUSEADDR 0x0004A   
#define SO_REUSEPORT 0x0000F

unsigned int htons(unsigned int hostshort);   
int socket (int domain, int type, int protocol);   
int accept (int s, struct sockaddr *addr, socklen_t *addrlen);  
int setsockopt (int s, int level, int optname, int *optval, int optlen);    
int bind (int s, struct sockaddr *name, int namelen);     
int listen (int s, int backlog);     
int recv (int s, char *buf, size_t bufLen, int flags);    
int send (int s, const char *buf, size_t bufLen, int flags); 

char *dirname(char *path);

char *basename(char *path); 

#if !(__GNUC__ && __amd64__)
 int getline(char **, size_t *, FILE *);
#endif

#ifndef INT_MIN
 #define INT_MIN -2147483647
#endif

#endif /* __CONFIG_H__ */
