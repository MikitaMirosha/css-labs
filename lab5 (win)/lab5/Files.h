#pragma once
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <strsafe.h>

using namespace std;

class Files
{
private:
	bool folders(char* mainFolder);
public:	
	char* mainFolder;
	int subfoldersAmount;
	char subfolders[MAX_PATH][100];
	
	bool files();
	bool folders();

	Files(char* mainFolder)
	{
		this->subfoldersAmount = 0;
		this->mainFolder = mainFolder;
	}
	~Files(void) {}
};

