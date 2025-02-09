#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define BUF_LEN 255

int main()
{
    int pipefd[2];


    if (pipe(pipefd) < 0) {
        perror("failed to create pipe");
        return 1;
    }
    int pid = fork();
    if (!pid)
    {
        char buf1[BUF_LEN] = "HELLO";
        int rc;
        rc = write(pipefd[1], buf1, BUF_LEN);
        printf("Sent: %s\n", buf1);
        exit(0);
    }
    else
    {
        wait(0);
        char buf2[BUF_LEN];
        int rc;
        rc = read(pipefd[0], buf2, BUF_LEN);
        printf("Received: %s\n", buf2);
    }
}