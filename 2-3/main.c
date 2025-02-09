#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <stdbool.h>

#define BUF_LEN 255

void *sendthread(void *arg)
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(*(int *)arg);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    while (1)
    {
        char data[BUF_LEN];
        scanf("%s", data);
        sendto(sock, data, BUF_LEN, 0, (struct sockaddr *)&addr, sizeof(addr));
    }
    close(sock);
    return 0;
}
void *recvthread(void *arg)
{
    int sock;
    struct sockaddr_in addr;

    char buf[BUF_LEN];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(*(int *)arg);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        if (recv(sock, buf, sizeof(buf), 0) < 0)
        {
            perror("recv");
            exit(1);
        }
        printf("%s\n", buf);
    }
}
int main(int argc, char *argv[])
{
    int sendport, recvport;
    if (argc != 3)
    {
        printf("Usage: %s <sendport> <recvport>\n", argv[0]);
        return 1;
    }
    sendport = atoi(argv[1]);
    recvport = atoi(argv[2]);
    pthread_t pthread1, pthread2;
    pthread_create(&pthread1, NULL, &sendthread, &sendport);
    pthread_create(&pthread2, NULL, &recvthread, &recvport);
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    return 0;
}