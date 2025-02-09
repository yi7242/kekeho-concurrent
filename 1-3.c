#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

void *thread1();
void *thread2();

int count = 0;
void *thread1()
{
    for (int i = 0; i < 10000; i++)
    {
        count++;
    }
}
int main(void)
{
    pthread_t pthread1, pthread2, pthread3, pthread4, pthread5;
    pthread_create(&pthread1, NULL, &thread1, NULL);
    pthread_create(&pthread2, NULL, &thread1, NULL);
    pthread_create(&pthread3, NULL, &thread1, NULL);
    pthread_create(&pthread4, NULL, &thread1, NULL);
    pthread_create(&pthread5, NULL, &thread1, NULL);

    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    pthread_join(pthread3, NULL);
    pthread_join(pthread4, NULL);
    pthread_join(pthread5, NULL);
    printf("count = %d\n", count);
    return 0;
}