// IOfuncs.cpp : Defines the exported functions for the DLL application.   

#include "stdafx.h"  
#include "IOfuncs.h"  
#include "../lab5/Threads.h"

namespace IOfuncs  
{
	DWORD Functions::runWriter(LPVOID lpParam)
	{
		HANDLE destinationFile;
		Threads* threadPointer = reinterpret_cast<Threads*>(lpParam);
	
		if(threadPointer->firstFile)
		{
			destinationFile = CreateFile(threadPointer->rootFolder, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			threadPointer -> firstFile = false;
		}
		else
		{
			destinationFile = CreateFile(threadPointer->rootFolder, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		}

		do
		{
			DWORD written;
			WaitForSingleObject(threadPointer->reading,INFINITE);
			WriteFile(destinationFile, threadPointer->buffer, threadPointer->fileSize.QuadPart, &written, NULL);

			if (!SetEvent(threadPointer->writing))
			{
				cout << "Ошибка 08: событие не установлено.\n"; 
				return -1;
			}
		} while(true);	

		return 0;
	}

	DWORD Functions::runReader(LPVOID lpParam)
	{
		Threads* threadPointer = reinterpret_cast<Threads*>(lpParam);

		do
		{
			if (!(threadPointer->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				threadPointer->fileSize.LowPart = threadPointer->findData.nFileSizeLow;
				threadPointer->fileSize.HighPart = threadPointer->findData.nFileSizeHigh;

				char fileToCopy[MAX_PATH];
				strcpy(fileToCopy,threadPointer->folder);
				strcat(fileToCopy,"\\");
				strcat(fileToCopy,threadPointer->findData.cFileName);

				cout << "[ " << threadPointer->findData.cFileName << " ]" << " - " << threadPointer->fileSize.QuadPart << " байт(а)\n";

				HANDLE sourceFile;
				threadPointer->buffer = new char[threadPointer -> fileSize.QuadPart];
	
				sourceFile = CreateFile(fileToCopy, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
					                    FILE_ATTRIBUTE_NORMAL, NULL);

				DWORD nRead;
				ReadFile(sourceFile, threadPointer->buffer, threadPointer->fileSize.QuadPart, &nRead, NULL);
				CloseHandle(sourceFile);
				
				if (!SetEvent(threadPointer->reading))
				{
					cout << "Ошибка 08: событие не установлено.\n"; 
					return -1;
				}
				WaitForSingleObject(threadPointer->writing, INFINITE);
			}
		}
		while (FindNextFile(threadPointer->hFind, &threadPointer->findData) != 0);

		if (!SetEvent(threadPointer->stopReading))
		{
			cout << "Ошибка 08: событие не установлено.\n"; 
			return -1;
		}
	
		return 0;
	}
}  