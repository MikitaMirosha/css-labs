#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE newProcess(string source, int pNum)
{
	char commandArgs[3]; 
	sprintf(commandArgs + 1, "%d", pNum);
	commandArgs[0] = ' ';

	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&sInfo, sizeof(sInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));
	sInfo.cb = sizeof(sInfo);

	if (!CreateProcess(source.c_str(), commandArgs,	NULL, NULL, false, 0, NULL, NULL, &sInfo, &pInfo))							
	{
		cout << "Error! Process is not created..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	return pInfo.hProcess;
}

void printProcess(string pNum)
{
	Sleep(500);
	for (int i = 0; i < pNum.length(); i++)
		cout << pNum[i] << " Process" << endl;
	Sleep(400);
}

int main(int argc, char** argv) 
{
	if (argc > 1) // ребенок
	{	
		HANDLE oEvent = OpenEvent(EVENT_ALL_ACCESS, false, "oEvent"); 
		string printInfo = argv[1];	// печать индекса события

		while (true) 
		{
			WaitForSingleObject(oEvent, INFINITE);
			printProcess(printInfo);
			SetEvent(oEvent); // сигнал есть событие
		}
	}
	else // родитель
	{
		cout << "Tap '+' to create new process" << endl;
		cout << "Tap '-' to kill last process" << endl;
		cout << "Tap 'q' to exit the program" << endl;

		HANDLE recEvent = CreateEvent(NULL, false, true, "recEvent");
		list <HANDLE> pList;

		while (true) 
		{
			WaitForSingleObject(recEvent, INFINITE);

			char input = NULL;

			do
			{
				switch ((input = _getch()))
				{
				case '+':
					pList.push_front(newProcess(string(argv[0]), pList.size() + 1));
					break;
				case '-':
					if (!pList.empty())
					{
						TerminateProcess(pList.front(), EXIT_SUCCESS);	// выходим из процесса
						CloseHandle(pList.front());	// закрываем дескриптор открытого объекта
						pList.pop_front();
					}
					else
					{
						cout << "List is empty! No processes created!" << endl;
					}
					break;
				case 'q':
					while (!pList.empty())
					{
						TerminateProcess(pList.front(), EXIT_SUCCESS);	// выходим из процесса
						CloseHandle(pList.front());	// закрываем дескриптор открытого объекта
						pList.pop_front();
					}
					CloseHandle(recEvent);
					return 0;
				}
			} while (input != 'q');
			SetEvent(recEvent);	// устанавливаем событие
			Sleep(100);
		}
	}
}

/* source.c_str() - имя исполняемого модуля
	commandArgs - командная строка
	NULL - дескриптор безопасности process (дескриптор не может быть унаследован)
	NULL - дескриптор безопасности thread (дескриптор не может быть унаследован)
	false - дескриптор параметра наследования от процесса, производящего запуск
	CREATE_NEW_CONSOLE - для нового процесса создается новая консоль
	NULL - указатель на блок конфигурации нового процесса (значения в виде строк с нулевым окончанием)
	NULL - рабочий каталог нового процесса (новый процесс создается с тем же самым текущим диском и каталогом, что и вызывающий процесс)
	&sInfo - свойства процесса (расположения окон и заголовка)
	&pInfo - информация о процессе (идентификаторы и дескрипторы нового процесса и основного его потока) */

	/* oEvent
	HANDLE - дескриптор
			oEvent - идентификатор объекта-события
			EVENT_ALL_ACCESS - указаны все возможные флаги доступа
			false - идентификатор не может наследоваться дочерними процессами
			"oEvent" - адрес символьной строки, содержащей имя объекта-события */

			/* CreateEvent
					NULL - атрибуты защиты
					false - флаг ручного сброса события (будет сброшено автоматически после того, как задача завершит ожидание этого события)
					true - флаг начального состояния события (объект-событие создается в отмеченном состоянии)
					"recEvent" - адрес имени объекта-события (имя можно выбирать любое) */
