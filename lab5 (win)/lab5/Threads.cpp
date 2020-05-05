#include "Threads.h"
#include "IOfuncs.h"

Threads::Threads(char* _folder, char* _rootFolder, WIN32_FIND_DATA _findData, HANDLE _hFind)
{
	findData = _findData;
	hFind = _hFind;
	strcpy(folder, _folder);
	strcpy(rootFolder, _rootFolder);
	firstFile = true;

	writing = CreateEvent(NULL, false, false, NULL);
	reading = CreateEvent(NULL, false, false, NULL);
	stopReading = CreateEvent(NULL, false, false, NULL);
	threadRead = CreateThread(NULL, 0, IOfuncs::Functions::runReader, this, 0, NULL);
	threadWrite = CreateThread(NULL, 0, IOfuncs::Functions::runWriter, this, 0, NULL);
	WaitForSingleObject(stopReading, INFINITE);

	cout << "\nЗапись успешно завершена.";
}

Threads::~Threads(void) {}