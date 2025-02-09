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
    const int num_threads = 5;
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread1, NULL);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("count = %d\n", count);
    return 0;
}