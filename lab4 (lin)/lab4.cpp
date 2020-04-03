#include <list>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

void* newThread(void* arg);

int main()
{
    void *(*createThread)(void *) = newThread;
    int state;
    void **state_addr = nullptr;

    pthread_t thread_ptr;
    pthread_mutex_t mutex;
    list<pthread_t> list_ptr;

    sigset_t sigset_wait;
    sigemptyset(&sigset_wait); // очищаем набор сигналов
    sigaddset(&sigset_wait, SIGCONT); // добавляем сигнал
    sigprocmask(SIG_BLOCK, &sigset_wait, NULL); // маска сигналов

    cout << "-------------------------" << endl;
    cout << "Tap '+' to new add thread" << endl;
    cout << "Tap '-' to delete thread" << endl;
    cout << "Tap 'q' to exit" << endl;
    cout << "-------------------------" << endl;

    while(true)
    {
        char input;
        rewind(stdin);
        cin.get(input);
        switch(input)
        {
            case '+':
            {
                state = pthread_create(&thread_ptr, NULL, createThread, &mutex); // создаем поток
                if (state == 0)
                {
                    list_ptr.push_back(thread_ptr);
                }
                else
                {
                    for (auto &i : list_ptr)
                    {
                        pthread_cancel(i); // посылаем запрос на завершение потока
                        pthread_join(i, state_addr); // ожидаем завершение потока
                    }
                    list_ptr.clear();
                    pthread_mutex_destroy(&mutex); // уничтожаем мьютекс
                    exit(0);
                }
                break;
            }
            case '-':
            {
                if (!list_ptr.empty())
                {
                    pthread_cancel(list_ptr.back()); // посылаем запрос на завершение потока
                    pthread_join(list_ptr.back(), state_addr); // ожидаем завершение потока
                    list_ptr.pop_back();
                }
                else
                {
                    cout << "List is empty!" << endl;
                }
                break;
            }
            case 'q':
            {
                while (!list_ptr.empty())
                {
                    pthread_cancel(list_ptr.back()); // посылаем запрос на завершение потока
                    pthread_join(list_ptr.back(), state_addr); // ожидаем завершение потока
                    list_ptr.pop_back();
                }
                list_ptr.clear();
                pthread_mutex_destroy(&mutex); // уничтожаем мьютекс
                return 0;
            }
        }
    }
}

void* newThread(void* arg)
{
    int threadID = 0;
    pthread_mutex_t mutex = *((pthread_mutex_t*)(arg));
    while(true)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); // запрещено завершение
        if(threadID == (int)pthread_self()) sleep(1); // задержка печати
        pthread_mutex_lock(&mutex); // установка мьютекса в положение "занято"
        cout << "Thread # " << (int)pthread_self() << endl;
        threadID = (int)pthread_self(); // возврат идентификатора
        pthread_mutex_unlock(&mutex); // установка мьютекса в положение "свободно"
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // разрешено завершение
        pthread_testcancel(); // проверка на ожидание завершения и завершение
    }
}

