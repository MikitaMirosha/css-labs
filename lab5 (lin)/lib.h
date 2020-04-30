#pragma once
#include <aio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
 
extern int8_t flag;
extern struct aiocb cb;
extern int countAmount;
extern char buffer[1024];
void* thread_read(void* args);
void* thread_write(void* args);

