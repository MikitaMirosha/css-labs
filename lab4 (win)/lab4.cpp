#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
#include <sstream>
#include <conio.h>
#include <Windows.h>

using namespace std;

DWORD counter = 0;

DWORD WINAPI newThread(CONST LPVOID p);

int main()
{
	stack<HANDLE> thread;
	CONST HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

	cout << "----------------------------" << endl;
	cout << "1. Tap '+' to add new thread" << endl;
	cout << "2. Tap '-' to delete thread" << endl;
	cout << "3. Tap 'q' to exit" << endl;
	cout << "----------------------------" << endl;

	while (true)
	{
		char input = NULL;
		do
		{
			switch (input = _getch())
			{
			case '+':
			{
				HANDLE nthread = CreateThread(NULL, 0, &newThread, mutex, 0, NULL);
				thread.push(nthread);
				break;
			}
			case '-':
			{
				if (!thread.empty())
				{
					counter--;
					TerminateThread(thread.top(), NO_ERROR);
					thread.pop();
				}
				else 
				{
					cout << "Stack is empty!" << endl;
				}
				break;
			}
			case 'q':
			{
				while (!thread.empty())
				{
					TerminateThread(thread.top(), NO_ERROR);
					thread.pop();
				}
				CloseHandle(mutex);
				ExitProcess(0);
			}
			}
		} while (input != 'q');
	}
}

DWORD WINAPI newThread(CONST LPVOID p)
{
	CONST HANDLE mutex = (CONST HANDLE)p;
	counter++;
	ostringstream stream;
	stream << counter;
	string out = "Thread # " + stream.str();

	while (true)
	{
		WaitForSingleObject(mutex, INFINITE);
		cout << out << endl;
		ReleaseMutex(mutex); // освобождение объекта
		Sleep(1500);
	}

	ExitThread(0);
}

/* CreateMutex(NULL, FALSE, NULL)
   NULL - атрибут безопасности
   FALSE - поток не получает право собственности на мьютекс
   NULL - имя мьютекса */

/* CreateThread(NULL, 0, &ThreadProc, mutex, 0, NULL)
   NULL - дескриптор защиты
   0 - начальный размер стека
   &newThread - функция потока
   mutex - параметр потока
   0 - опции создания
   NULL - идентификатор потока
*/

/* TerminateThread(thread.top(), NO_ERROR)
   thread.top() - дескриптор потока
   NO_ERROR - код завершения
*/