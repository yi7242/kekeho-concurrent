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
int op_counter = 0;
int send_port[3] = {5000, 5001, 5002};
void *sendthread()
{
    int sock;
    struct sockaddr_in addr[3];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    for (int i = 0; i < 3; i++)
    {
        addr[i].sin_family = AF_INET;
        addr[i].sin_port = htons(send_port[i]);
        addr[i].sin_addr.s_addr = inet_addr("127.0.0.1");
    }
    while (1)
    {
        char data[BUF_LEN];
        scanf("%s", data);
        if ((strcmp(data, "?") == 0))
        {
            printf("Counter: %d\n", op_counter);
        }
        else if ((strcmp(data, "+") != 0) && (strcmp(data, "-") != 0))
        {
            printf("Invalid command\n");
            continue;
        }
        for (int i = 0; i < 3; i++)
        sendto(sock, data, BUF_LEN, 0, (struct sockaddr *)&addr[i], sizeof(addr[i]));
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
        if (strcmp(buf, "+") == 0)
        {
            op_counter++;
            // printf("counter: %d\n", op_counter);
        }
        else if (strcmp(buf, "-") == 0)
        {
            op_counter--;
            // printf("counter: %d\n", op_counter);
        }
    }
}
int main(int argc, char *argv[])
{
    int recvport;
    if (argc != 2)
    {
        printf("Usage: %s <recvport>\n", argv[0]);
        return 1;
    }
    recvport = atoi(argv[1]);
    pthread_t pthread1, pthread2;
    pthread_create(&pthread1, NULL, &sendthread, NULL);
    pthread_create(&pthread2, NULL, &recvthread, &recvport);
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    return 0;
}