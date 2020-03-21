#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <limits>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <windows.h>

using namespace std;

#define MIN_SIZE 0
#define MAX_SIZE 70

#undef max

void Child();
void Parent(char*);

int main(int argc, char* argv[]) 
{
	if (argc > 1)
		Child();
	else 
		Parent(argv[0]);
	return 0;
}

void Parent(char* enter)
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(STARTUPINFO));
	sInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&pInfo, sizeof(PROCESS_INFORMATION));

	if (!CreateProcess(enter, (LPSTR)"Child process", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sInfo, &pInfo))
	{
		cout << "Process failed..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	HANDLE Play = CreateSemaphore(NULL, 0, 1, "Play");
	HANDLE WINAPI ProjectedFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, MIN_SIZE, MAX_SIZE, "ProjectedFile");
	LPVOID p = MapViewOfFile(ProjectedFile, FILE_MAP_ALL_ACCESS, 0, 0, 0); // oтображаем представление чтения/записи файла
	char* outputString = (char*)p;
	bool inputString = true;
	int currentStringPosition = 0;

	string bufferString;
	bufferString.resize(MAX_SIZE, '\0'); // выделяем на 70 по MAX_SIZE

	while (true)
	{
		if (inputString)
		{
			currentStringPosition = 0;
			cout << "Server: Enter string" << endl;
			getline(cin, bufferString);
			inputString = false;
		}

		string tempBuffer;
		int newStringLength = 0;
		tempBuffer.append(bufferString, 0, MAX_SIZE - 1); // добавляет символы в конец строки
		currentStringPosition = tempBuffer.length();

		strcpy(outputString, const_cast<char*>(tempBuffer.c_str()));
		outputString[tempBuffer.length()] = '\0';

		tempBuffer.clear();
		newStringLength = bufferString.length() - currentStringPosition;
		if (newStringLength > 0)
			tempBuffer.append(bufferString, currentStringPosition, newStringLength);
		bufferString = tempBuffer;

		ReleaseSemaphore(&Play, 1, NULL); // увеличивает значение счетчика
		WaitForSingleObject(&Play, INFINITE);

		if (bufferString.empty())
		{
			inputString = true;
			cout << "Tap '0' to exit" << endl;
			cout << "Tap any another key to continue: ";
			if (cin.get() == '0')
			{
				CloseHandle(pInfo.hThread);
				CloseHandle(pInfo.hProcess);
				return;
			}
			else
			{
				bufferString.clear();
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}
}

void Child() 
{
	HANDLE Play = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Play"); 
	HANDLE WINAPI ProjectedFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "ProjectedFile");  // открываем проецируемый файл
	LPVOID p = MapViewOfFile(ProjectedFile, FILE_MAP_ALL_ACCESS, 0, 0, 0); // oтображаем представление чтения/записи файла
	char* outputString = (char*)p;

	while (WaitForSingleObject(&Play, INFINITE))
	{
		
		cout << "Client received: " << outputString << endl;
		ReleaseSemaphore(&Play, 1, NULL); // увеличивает значение счетчика
		Sleep(3500);
	}

	CloseHandle(&Play);
	UnmapViewOfFile(&p); // отменяет отображение представления файла
	CloseHandle(&ProjectedFile);
	return;
}


