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

	// �������� ����� ���� � �����
	StringCchLength((TCHAR*)mainFolder, MAX_PATH, &argLength);
	if (argLength > (MAX_PATH - 3))
	{
		cout << "\n������ 03: ���� � ����� ������� �������.\n";
		return false;
	}
	
	StringCchCopy(folderSize, MAX_PATH, (TCHAR*)mainFolder);
	StringCchCat(folderSize, MAX_PATH, TEXT("\\*"));
	
	// ���������� ������ �����
	hFindFile = FindFirstFile(folderSize, &findData);
	if (INVALID_HANDLE_VALUE == hFindFile) 
	{
		cout << "\n������ 04: ����� �� �������.\n";
		return false;
	} 
   
	// ����� ����� �� ����� 
	cout << "������� �����: \n" << mainFolder;
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
		// �������� ����� ���� � �����
		StringCchLength((TCHAR*)subfolders[i], MAX_PATH, &argLength);
		if (argLength > (MAX_PATH - 3))
		{
			cout << "\n������ 03: ���� � ����� ������� �������.\n";
			return false;
		}
	
		cout << "\n\n����� [ txt ] � �������: \n\n";
		
		StringCchCopy(folderSize, MAX_PATH, (TCHAR*)subfolders[i]);
		StringCchCat(folderSize, MAX_PATH, TEXT("\\*"));
	
		// ���������� ������� ����� � �����
		hFindFile = FindFirstFile(folderSize, &findData);
		if (INVALID_HANDLE_VALUE == hFindFile) 
		{
			cout << "\n������ 06: ���� � ����� �� ������.\n";
			return false;
		}
   
		// ������ ������ � �����
		char rootFolder[MAX_PATH];
		strcpy(rootFolder, subfolders[i]);
		strcat(rootFolder, ".txt");
		Threads fThread(subfolders[i], rootFolder, findData, hFindFile);
	}
	FindClose(hFindFile);
	return true;
}