#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main() {
    int pid = fork();
    if (!pid) {
        printf("Hello, World! from process #1\n");
        exit(0);
    }
    else {
        printf("Hello, World! from process #2\n");
        wait(0);
    }
}