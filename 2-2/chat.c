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
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    int len;
    int sock;

    sock0 = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(*(int*)arg);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock0, 5);
    printf("Waiting for connection\n");
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    printf("Connected\n");
    while (1)
    {
        char data[BUF_LEN];
        scanf("%s", data);
        sendto(sock, data, BUF_LEN,0, (struct sockaddr *)&addr, sizeof(addr));
    }
    close(sock);
    close(sock0);
    return 0;
}
void *recvthread(void *arg)
{   
    int sock2;
    struct sockaddr_in server;

    char buf[BUF_LEN];

    sock2 = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(*(int*)arg);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    while (connect(sock2, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Waiting for connection\n");
        sleep(1);
    }
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        if (recv(sock2, buf, sizeof(buf), 0) < 0)
        {
            perror("recv");
            exit(1);
        }
        printf("%s\n", buf);
    }
    close(sock2);
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