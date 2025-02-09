#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_LEN 255

int main()
{
    int pid = fork();
    if (!pid)
    {
        int sock;
        struct sockaddr_in addr;

        sock = socket(AF_INET, SOCK_DGRAM, 0);

        addr.sin_family = AF_INET;
        addr.sin_port = htons(12345);
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
    else
    {

        int sock;
        struct sockaddr_in addr;

        char buf[BUF_LEN];

        sock = socket(AF_INET, SOCK_DGRAM, 0);

        addr.sin_family = AF_INET;
        addr.sin_port = htons(12346);
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
}