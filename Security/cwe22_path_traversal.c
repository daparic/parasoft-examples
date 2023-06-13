#include "config.h"

#define BUFFER_SIZE 1024
#define PORT 8080

int get_message(char * const buffer, const size_t buffer_size, size_t * const bytesread)
{
    int server_fd = -1;
    struct sockaddr_in address;
    int opt = 1;
    int new_socket = 0;
    size_t addrlen = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(-1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("set socket option failed");
        exit(-1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(-1);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(-1);
    }

    addrlen = sizeof(address);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(-1);
    }

    *bytesread = recv(new_socket, buffer, buffer_size, 0);

    return new_socket;
}

size_t list_directory(const char * const path, const size_t path_size, char * const buffer, const size_t buffer_size)
{
    size_t bytes_written = 0;
    DIR * dr = NULL;
    struct dirent * de = NULL;
    char * bufptr = NULL;

    char *dirpath = malloc(path_size + 3);

    memset(&dirpath, 0, path_size + 3);
    dirpath[0] = '.';
    dirpath[1] = '/';
    strncat(dirpath + 2, path, path_size - 1);

    /*
     * === CWE 22 :: Path Traversal Vulnerability ===
     *
     * -- DESCRIPTION --
     *   Path traversal vulnerabilities occur when the software allows external
     *   input to construct a path intended to identify a file or directory. If
     *   the software does not neutralize special elements such as '..', '/', and
     *   '~' from the path, then the software could be used to access the file
     *   system in unexpected ways. This can result in information leak and
     *   external file access that can be combined with other vulnerabilites to
     *   enable complete access, denial of service, and data loss.
     *
     *   In this case the program is expecting to display directories relative of
     *   the current working directory. However, an attacker can supply '..' to
     *   list directories outside this scope in order to remotely traverse the
     *   entire file system.
     *
     * -- KNOWN CASES --
     *   CVE-2019-13623 : NSA Ghidra
     *   CVE-2019-9445  : Google Android
     *   CVE-2019-8943  : WordPress
     *   CVE-2019-5624  : Rapid7 Metasploit
     *   CVE-2019-3397  : Atlassian Bitbucket
     *   CVE-2019-1836  : Cisco Nexus 9000 Series Fabric Switches
     *
     * -- DETECTED BY --
     *   C++Test : BD-SECURITY-TDFNAMES
     *             MISRA2012-DIR-4_14_f-2
     */
    dr = opendir(dirpath);

    if (dr == NULL) {
        perror("opendir failed");
        exit(-1);
    }

    bufptr = buffer;

    while ((de = readdir(dr)) != NULL) {
        char * entry_name = de->d_name;
        size_t entry_size = strlen(entry_name);

        if (bufptr + entry_size + 1 > buffer + buffer_size) {
            fputs("list directory buffer full", stderr);
            break;
        }

        strncat(bufptr, entry_name, entry_size);
        bufptr += entry_size;
        *bufptr = '\n';
        ++bufptr;
        bytes_written += entry_size + 1;
    }

    if (closedir(dr) != 0) {
        perror("closedir failed");
        exit(-1);
    }

    return bytes_written;
}


int main(int argc, char * argv[])
{
    char req[BUFFER_SIZE] = {0};
    size_t req_size = 0;

    int new_socket = get_message(req, BUFFER_SIZE, &req_size);

    char rsp[BUFFER_SIZE] = {0};
    int rsp_size = list_directory(req, req_size, rsp, BUFFER_SIZE);

    send(new_socket, rsp, rsp_size, 0);

    return 0;
}
