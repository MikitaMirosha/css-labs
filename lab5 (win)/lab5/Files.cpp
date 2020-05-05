#include "Files.h"
#include"Threads.h"

bool Files::folders()
{
	return folders(mainFolder);
}

bool Files::folders(char* mainFolder)
{
	size_t argLength;
	DWORD errors = 0;
	WIN32_FIND_DATA findData;
	TCHAR folderSize[MAX_PATH];
	HANDLE hFindFile = INVALID_HANDLE_VALUE;

	// Проверка длины пути к папке
	StringCchLength((TCHAR*)mainFolder, MAX_PATH, &argLength);
	if (argLength > (MAX_PATH - 3))
	{
		cout << "\nОшибка 03: путь к папке слишком длинный.\n";
		return false;
	}
	
	StringCchCopy(folderSize, MAX_PATH, (TCHAR*)mainFolder);
	StringCchCat(folderSize, MAX_PATH, TEXT("\\*"));
	
	// нахождение первой папки
	hFindFile = FindFirstFile(folderSize, &findData);
	if (INVALID_HANDLE_VALUE == hFindFile) 
	{
		cout << "\nОшибка 04: папка не найдена.\n";
		return false;
	} 
   
	// вывод папки на экран 
	cout << "Рабочая папка: \n" << mainFolder;
	do
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!strcmp(findData.cFileName, ".") || !strcmp(findData.cFileName, ".."))
				continue;

			cout << "\n" << "[ " << findData.cFileName << " ]";
			strcat(strcat(strcpy(this->subfolders[subfoldersAmount], mainFolder), "\\"), findData.cFileName);
			this->subfoldersAmount++;
		}
	}
	while (FindNextFile(hFindFile, &findData) != 0);

	FindClose(hFindFile);
	return true;
}

bool Files::files()
{
	size_t argLength;
	LARGE_INTEGER fileSize;
	WIN32_FIND_DATA findData;
	TCHAR folderSize[MAX_PATH];
	HANDLE hFindFile = INVALID_HANDLE_VALUE;

	for(int i = 0; i < subfoldersAmount; i++)
	{
		// Проверка длины пути к папке
		StringCchLength((TCHAR*)subfolders[i], MAX_PATH, &argLength);
		if (argLength > (MAX_PATH - 3))
		{
			cout << "\nОшибка 03: путь к папке слишком длинный.\n";
			return false;
		}
	
		cout << "\n\nПапка [ txt ] с файлами: \n\n";
		
		StringCchCopy(folderSize, MAX_PATH, (TCHAR*)subfolders[i]);
		StringCchCat(folderSize, MAX_PATH, TEXT("\\*"));
	
		// нахождение первого файла в папке
		hFindFile = FindFirstFile(folderSize, &findData);
		if (INVALID_HANDLE_VALUE == hFindFile) 
		{
			cout << "\nОшибка 06: файл в папке не найден.\n";
			return false;
		}
   
		// Список файлов в папке
		char rootFolder[MAX_PATH];
		strcpy(rootFolder, subfolders[i]);
		strcat(rootFolder, ".txt");
		Threads fThread(subfolders[i], rootFolder, findData, hFindFile);
	}
	FindClose(hFindFile);
	return true;
}