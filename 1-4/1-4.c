#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

void *thread1();
void *thread2();

int count = 0;
pthread_mutex_t mutex;

void *thread1()
{
    for (int i = 0; i < 10000; i++)
    {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
    }
}
int main(void)
{
    pthread_mutex_init(&mutex, NULL);
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