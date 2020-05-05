#pragma once
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <strsafe.h>
#include <windows.h>

using namespace std;

class Threads
{
private:
	HANDLE threadRead;
	HANDLE threadWrite;
public:
	char* buffer;
	bool firstFile;
	char file[MAX_PATH];
	char folder[MAX_PATH];
	char rootFolder[MAX_PATH];

	HANDLE hFind;
	HANDLE reading;
	HANDLE writing;
	HANDLE stopReading;
	LARGE_INTEGER fileSize;
	WIN32_FIND_DATA findData;

	Threads(char* _folder, char* _rootFolder, WIN32_FIND_DATA _findData, HANDLE _hFind);
	~Threads(void);
};

