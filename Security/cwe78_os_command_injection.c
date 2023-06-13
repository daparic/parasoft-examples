#include "config.h"

#define USERNAME_MAX 32
#define BUFFER_SIZE 4096
#define PORT 8080

int get_message(char * const buffer, const size_t buffer_size, size_t * const bytesread)
{
    int server_fd = -1;
    int opt = 1;
    struct sockaddr_in address;
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

#define COMMAND_BUFFER_SIZE USERNAME_MAX + 8

char * list_groups(char const * const username,
                   const size_t username_size,
                   size_t * const bytes_written)
{
    char * buffer = malloc(BUFFER_SIZE + 1);
    char * command = 0;
    int command_size = 0;
    FILE * f = NULL;
    char * buffptr = 0;

    if (buffer == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    memset(buffer, 0, BUFFER_SIZE + 1);

    /* Construct groups command. */
    command = malloc(COMMAND_BUFFER_SIZE);

    if (command == NULL) {
        perror("malloc failed");
        exit(-1);
    }

    command_size = sprintf(command, "groups %s", username);

    if (command_size < 0) {
        perror("sprintf error");
        exit(-1);
    } else if (command_size > COMMAND_BUFFER_SIZE) {
        fputs("sprintf buffer overflow detected", stderr);
        exit(-1);
    }
    
    /* Run command and collect input. */

    /*
     * === CWE 78 :: OS Command Injection ===
     *
     * -- DESCRIPTION --
     *   OS command injection occurs when software allows external input to
     *   construct a command intended to run a single program. If the software
     *   does not sanitize this input, then the input could be used to execute
     *   multiple commands or introduced unintended arguments. This can result in
     *   information leak to complete remote code execution.
     *
     *   In this case the program receive a user name from a TCP socket and
     *   respond with the groups that user is a member of on the system. However,
     *   an attacker can provide a user name with a semicolon appended and then
     *   supply further shell commands resulting in arbitrary code execution.
     *
     * -- KNOWN CASES --
     *   CVE-2019-9740  : Python urllib
     *   CVE-2019-9204  : Nagios
     *   CVE-2019-9121  : Motorola C1 & M2 firmware
     *   CVE-2019-7632  : LifeSize Team, Room, Passport, & Networker
     *   CVE-2019-1781  : Cisco FXOS & NX-OS
     *
     * -- DETECTED BY --
     *   C++Test : BD-SECURITY-TDCMD
     */
    if ((f = popen(command, "r")) == NULL) {
        perror("popen failed");
        exit(-1);
    }

    buffptr = buffer;

    while (1) {
        if (fgets(buffptr, BUFFER_SIZE - strlen(buffer), f) == NULL) {
            if (feof(f)) {
                break;
            } else if (ferror(f)) {
                perror("fgets failed");
                exit(-1);
            }
        }

        if (strlen(buffer) >= BUFFER_SIZE) {
            break;
        }

        buffptr = buffer + strlen(buffer);
    } 

    if (pclose(f) == -1) {
        perror("pclose failed");
        exit(-1);
    }

    free(command);

    *bytes_written = strlen(buffer);

    return buffer;
}
 
int main(int argc, char* argv[])
{
    char req[USERNAME_MAX] = {0};
    size_t req_size = 0;

    int new_socket = get_message(req, USERNAME_MAX, &req_size);

    char * rsp;
    size_t rsp_size = 0;
    rsp = list_groups(req, req_size, &rsp_size);

    send(new_socket, rsp, rsp_size, 0);

    free(rsp);
    
    return 0;
}
