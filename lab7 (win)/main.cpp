#include <string>
#include <iostream>
#include "FileManager.h"

using namespace std;
using namespace filemanager;

int main()
{
	system("Color 70");
	FileManager fileManager;

	cout << "----------------------------------------" << endl;
	cout << "------the file system welcomes you------" << endl;
	cout << "----------------------------------------" << endl;
	cout << endl << "for more information enter [help]" << endl << endl;

	while (true)
	{
		cout << "> ";
		string command;
		cin >> command;
		if (command == "dir")
		{
			fileManager.ShowFile();
		}
		else if (command == "add")
		{
			fileManager.AddFile();
		}
		else if (command == "del")
		{
			fileManager.DeleteFile();
		}
		else if (command == "res")
		{
			while (true)
			{
				cout << endl << "the data will be deleted" << endl;
				cout << "are you sure?" << endl;
				cout << "press 'y' or 'n'" << endl;
				string reset;
				cin >> reset;
				if (reset == "y")
				{
					fileManager.ResetSpace();
					break;
				}
				else if (reset == "n")
				{
					break;
				}
				else
				{
					cout << "command not found" << endl;
					break;
				}
			}
		}
		else if (command == "open")
		{
			fileManager.OpenFile();
		}
		else if (command == "cls")
		{
			system("cls");
		}
		else if (command == "help")
		{
			fileManager.HelpInfo();
		}
		else if (command == "exit")
		{
			break;
		}
		else
		{
			cout << "command not found" << endl;
		}
	}
	return 0;
}