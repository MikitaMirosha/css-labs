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
        initscr();
        while(true)
        {
        switch (fork())
        {
                case -1:
                        exit(1);
                case 0: // дочерний процесс
                {
                        void *makeChild = giveMem(sizeof(100));
                        char *getT = getTime();
                        memcpy(makeChild, (void *)getT, strlen(getT));
                        printw("Child time: %s\n", makeChild);
                        //refresh();
                        getchar();
                        //break;
                }
                default: // родительский процесс
                {

                        printw("Parent time: %s", getTime());
                        //refresh(); // копирует stdscr в curscr; stdscr - буфер для данных из mvprintw;
                        // curscr - изображение, выводимое на экран
getchar();
                        endwin();
                                    refresh();
                        wait(NULL);
                }
        }
        }

        endwin(); // очищает все ресурсы для ncurses 
    return 0;
}

                                                                                                                                                                                                 62,0-1      Внизу
