#include "Files.h"
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <strsafe.h>

using namespace std;

int _tmain(int argc, TCHAR *argv[])
{
	setlocale(LC_ALL, "Russian");
	char folder[] = { "C:\\Users\\User\\Desktop\\bsuir\\4thterm\\�����\\labs\\lab5\\files" };
	Files find(folder);
	
	if(!find.folders())
	{
		cout << "\n������ 01: ���������� �� �������.\n";
		cin.get();
		return -1;
	}
	if(!find.files())
	{
		cout << "\n������ 02: ���� �� ������.\n";
		cin.get();
		return -1;
	}

	cout << "\n\n���������� ����������: " << find.subfoldersAmount;
	cin.get();
	return 0;
}