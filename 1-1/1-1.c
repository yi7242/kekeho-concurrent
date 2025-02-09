#include <stdio.h>
#include <pthread.h>

void *thread1() {
  printf("Hello, World! from thread #1\n");
  return 0;
}
void *thread2() {
  printf("Hello, World! from thread #2\n");
  return 0;
}

int main(void) {
  pthread_t pthread1, pthread2;
  pthread_create( &pthread1, NULL, &thread1, NULL);
  pthread_create( &pthread2, NULL, &thread2, NULL);
  pthread_join(pthread1, NULL);
  pthread_join(pthread2, NULL);
  return 0;
}