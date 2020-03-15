#include <vector>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <ncurses.h>
#include <iostream>
#define PROCESS_AMOUNT 5
using namespace std;

bool Print = true;
vector<pid_t> pInfo;
char parentPID[256];
struct sigaction printSignalFromChild;
int i = 0;

int charGet()
{
   struct termios oldAttr, newAttr; // общий терминальный интерфейс
   tcgetattr(STDIN_FILENO, &oldAttr); // получить параметры
   newAttr = oldAttr;
   newAttr.c_lflag &= ~(ICANON | ECHO); // режим локали (+, -)
   tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
   int symbol;
   symbol = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);
   return symbol;
}

/* STDIN_FILENO стандартный ввод файлового дескриптора (0 для консоли)
// TCSANOW - изменения немедленно начинают работать
// ICANON - канонический режим (спец символы, строчная буферизация)
// разрешение на исполнение функций забоя и стирания строк
// ECHO - отображает вводимые символы
// tcgetattr - получить параметры
// tcsetattr - меняет параметры в терминале
// c_lflag - режим локали, обработка строк

// сигналы:
// to child: SIGUSR1 - начать печать
// to child: SIGUSR2 - завершить процесс
// from child: SIGUSR1 - завершить печать */

void setPrint(int sig)
{
   if (pInfo.size())
    {
      if (++i >= pInfo.size())
      {
         i = 0;
      }
      sleep(150);
      kill(pInfo[i], SIGUSR1);  // SIGUSR1 - завершить печать
   }
   else
   {
      Print = true; // печать активна
   }
}

void initSignal()
{
   printSignalFromChild.sa_handler = setPrint;
   printSignalFromChild.sa_flags =  SA_RESTART;
   sigaction(SIGUSR1, &printSignalFromChild, NULL);
   // в SIGUSR1 устанавливается printSignalFromChild
}

/* sa_handler - обработка сигнала
// sa_flags - битовая маска
// SA_RESTART - библиотечная функция может продолжиться, или она может возвратить отказ с кодом ошибки EINTR */

void makeProcess()
{
   pInfo.push_back(fork());
   if (pInfo.back() == 0)
   {
      char ID[10];
      sprintf(ID, "%ld", pInfo.size()); // ld
      if (execlp("./c", ID, parentPID, NULL) == -1)
      {
         pInfo.pop_back();
         cout << "Process failed..." << endl;
      }
   }
}
/* execlp:
// l - аргументы ком строки передаются в форме списка
// p - поиск файла не только в текущем каталоге, но и в PATH */

void deleteProcess()
{
   kill(pInfo.back(), SIGUSR2);
   waitpid(pInfo.back(), NULL, 0);
   pInfo.pop_back();
}

void showMenu()
{
   cout << "Tap '+' to create new process" << endl;
    cout << "Tap '-' to kill last process" << endl;
    cout << "Tap 'q' to exit the program" << endl;
}

int main()
{
   showMenu();
   initSignal();
   sprintf(parentPID, "%d", getpid());

    while (true)
    {
      switch(charGet())
      {
         case '+':
         {
            if(pInfo.size() < PROCESS_AMOUNT)
            {
               makeProcess();
               if (Print == true)
                {
                  sleep(150);
                  kill(pInfo.back(), SIGUSR1);
                  Print = false;
               }
            }
            break;
         }

         case '-':
         {
            if (!pInfo.empty())
            {
               if (i == pInfo.size() - 1)
               {
                  deleteProcess();
                  raise(SIGUSR1);
               } else
               {
                  deleteProcess();
               }
            }
            break;
         }

         case 'q':
         {
            while (!pInfo.empty())
            {
               deleteProcess();
            }
            cout << "\nPress any key...\n";
            charGet();
            return 0;
         }
      }
    }

    return 0;
}


