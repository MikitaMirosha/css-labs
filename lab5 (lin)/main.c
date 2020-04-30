#include "lib.h"
#include <locale.h>
#include <pthread.h>

int main()
{
  setlocale(LC_ALL, "Russian");
  pthread_t writing, reading;
  pthread_create(&writing, NULL, &thread_write, NULL);
  pthread_create(&reading, NULL, &thread_read, NULL);
  pthread_join(writing, NULL);
  pthread_join(reading, NULL);
  return 0;
}

