#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

void *thread1();
void *thread2();
void setter(int value);

int x = 0;
void *thread1()
{
    int prev;
    while (prev != 20)
    {
        if (prev != x)
        {
            printf("#Reader: Read x = %d\n", x);
            prev = x;
        }
    }
    return 0;
}
void *thread2()
{
    x = 10;
    printf("#Writer: Set x = %d\n", 10);
    sleep(1);
    x = 20;
    printf("#Writer: Set x = %d\n", 20);
    sleep(1);
    return 0;
}

int main(void)
{
    pthread_t pthread1, pthread2;
    pthread_create(&pthread1, NULL, &thread1, NULL);
    pthread_create(&pthread2, NULL, &thread2, NULL);
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    return 0;
}