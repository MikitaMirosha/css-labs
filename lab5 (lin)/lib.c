#include "lib.h"

struct aiocb cb;
char buffer[1024];
int8_t flag = FALSE;
int countAmount = 5;

void* thread_write(void* args)
{
    // флаги открытия файлов
    int flags = O_CREAT | O_APPEND | O_WRONLY;
    // O_APPEND - The file is opened in append mode
    // O_CREAT - If pathname does not exist, create it as a regular file
    // O_WRONLY - Open for writing only
     int resultFile = open("result", flags, 0644);
    // 0644 - read/write mode for linux
  
    if(-1 == resultFile)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while(countAmount > 0)
    {
        while(TRUE != flag)
	    {
	        usleep(100);
	    }

    cb.aio_nbytes = strlen(buffer); // буфер для организации записи
    cb.aio_fildes = resultFile; // формирование файлового дескриптора ввода/вывода
      
    if(-1 == aio_write(&cb)) // запрос на запись
	{
	    perror("aio_write");
	    close(resultFile);
	    exit(EXIT_FAILURE);
	}

    flag = FALSE;
    }
    printf("Дадзеныя запісаныя паспяхова\n");
    pthread_exit(NULL);
}

void* thread_read(void *args)
{
    char fileName[256];
    memset(&cb, 0, sizeof(cb));
    cb.aio_offset = 0;
    cb.aio_buf = buffer;
  
    for (int i = 1; i < 6; i++)
    {
        while(TRUE == flag)
	    {
	        usleep(100);
	    }

        sprintf(fileName, "%d", i);
        int currentFile = open(fileName, O_RDONLY); // read only

        if(-1 == currentFile)
	    {
	        perror("open");
	        exit(EXIT_FAILURE);
	    }

        cb.aio_nbytes = 256;
        cb.aio_fildes = currentFile;  // формирование файлового дескриптора ввода/вывода
      
        if(-1 == aio_read(&cb))
	    {
	        perror("aio_read");
	        close(currentFile);
	        exit(EXIT_FAILURE);
	    }  
	
	    // чтение всех файлов
	    while(aio_error(&cb) == EINPROGRESS) {}
        countAmount--;
        flag = TRUE;
      
        printf("%s", buffer); 
        close(currentFile);
    }
    printf("Дадзеныя прачытаныя паспяхова\n");
    pthread_exit(NULL);
}
