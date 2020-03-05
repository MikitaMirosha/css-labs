#include <ctime>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <curses.h>
#include <sys/mman.h>
#include <sys/wait.h>

char *getTime() 
{
    time_t timer = time(nullptr);
    tm *local = localtime(&timer);
    return asctime(local);
}

void *giveMem(size_t size) 
{
        int prot = PROT_READ | PROT_WRITE; 
        int flags = MAP_SHARED | MAP_ANONYMOUS; 
        return mmap(NULL, size, prot, flags, -1, 0);
}
// PROT_READ | PROT_WRITE - данные можно вписывать и считывать
// MAP_SHARED - для работы с другими процессами
// MAP_ANONYMOUS - отображение не резервируется ни в каком файле

int main() 
{
        void *childPID = giveMem(sizeof(int));
        void *makeChild = giveMem(sizeof(100));

        switch (fork())
        {
                case -1:
                        exit(1);
                case 0: // дочерний процесс
                {
                        char *getT = getTime();
                        int getPID = getpid();
                        memcpy(makeChild, (void *)getT, strlen(getT));
                        memcpy(childPID, (void *)&getPID, sizeof(int));
                        break;
                }
                default: // родительский процесс
                {
                        wait(NULL); 
                        initscr(); 
                        mvprintw(0, 0, "Child time: %sChild process PID: %d", makeChild, *(int *)childPID);
                        mvprintw(2, 0, "Parent time: %sParent process PID: %d", getTime(), getpid());
                        refresh(); // копирует stdscr в curscr; stdscr - буфер для данных из mvprintw;
                        // curscr - изображение, выводимое на экран
                        getchar();
                        endwin(); // очищает все ресурсы для ncurses 
                }
        }
    return 0;
}
