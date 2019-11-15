#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>

int main()
{
    time_t clock;
    char dataSending[1025];
    int clintListn = 0, clintConnt = 0;
    struct sockaddr_in ipOfServer;
    clintListn = socket(AF_INET, SOCK_STREAM, 0);
    memset(&ipOfServer, '0', sizeof(ipOfServer));
    memset(dataSending, '0', sizeof(dataSending));
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
    ipOfServer.sin_port = htons(2017); //  port number of running server
    bind(clintListn, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer));
    listen(clintListn, 20);

    while (1)
    {
        printf("New client connected\n");
        clintConnt = accept(clintListn, (struct sockaddr *)NULL, NULL);

        clock = time(NULL);
        snprintf(dataSending, sizeof(dataSending), "%.24s\r\n", ctime(&clock));
        write(clintConnt, dataSending, strlen(dataSending));
        close(clintConnt);
        sleep(1);
    }

    return 0;
}