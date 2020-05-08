#include <string>
#include <conio.h>
#include <fstream>
#include <iostream>
#include "FileManager.h"

using namespace std;
using namespace filemanager;

// физический носитель
const char FileManager::FHYSICAL_SPACE_NAME[] = "space.bin";

void FileManager::HelpInfo()
{
	cout << endl << "----------------------------------------" << endl;
	cout << "[add] - to add a file" << endl;
	cout << "[del] - to delete a file" << endl;
	cout << "[open] - to open a file" << endl;
	cout << "[dir] - to show the directory" << endl;
	cout << "[cls] - to clear the screen" << endl;
	cout << "[res] - to reset the virtual space" << endl;
	cout << "[exit] - to exit the program" << endl;
	cout << "----------------------------------------" << endl << endl;
}

FileManager::FileManager()
{
	// инициализация свободного места в виртуальном пространстве и заполнение нулями
	this->freeSpot = sizeof(MemoryBlock) * BLOCKS_QUANTITY;
	SecureZeroMemory(this->FMDTField, this->FILES_QUANTITY * sizeof(FileMetaDataTableField));
	this->sourceFile.open(this->FHYSICAL_SPACE_NAME, ios::binary | ios::in | ios::out | ios::ate);

	if (!this->sourceFile.is_open())
	{
		this->sourceFile.open(this->FHYSICAL_SPACE_NAME, ios::binary | ios::out | ios::ate);

		const int emptyFileSize = this->BLOCKS_QUANTITY * sizeof(MemoryBlock) + this->FMDT_SIZE;
		byte setArray[emptyFileSize];
		SecureZeroMemory(setArray, emptyFileSize);
		this->sourceFile.write(setArray, emptyFileSize);

		this->sourceFile.close();
		this->sourceFile.open(this->FHYSICAL_SPACE_NAME, ios::binary | ios::in | ios::out | ios::ate);
	}
	else
	{
		this->sourceFile.seekg(0);
		this->sourceFile.read((byte*)this->FMDTField, sizeof(FileMetaDataTableField) * this->FILES_QUANTITY);
	}
}

FileManager::~FileManager()
{
	this->sourceFile.close();
}

// вывод всех файлов на экран
void FileManager::ShowFile()
{
	cout << endl << "files: " << endl;
	for (int i = 0; i < this->FILES_QUANTITY; i++)
	{
		if (this->FMDTField[i].initialOffset != 0)
		{
			cout << '>' << this->FMDTField[i].fileName << endl;
		}
	}
	cout << endl;
}

// удаление всей информации из пространства и из FMDT, запись нулей в файл
void FileManager::ResetSpace()
{
	this->freeSpot = sizeof(MemoryBlock) * BLOCKS_QUANTITY;
	SecureZeroMemory(this->FMDTField, this->FILES_QUANTITY * sizeof(FileMetaDataTableField));

	this->sourceFile.close();
	this->sourceFile.open(this->FHYSICAL_SPACE_NAME, ios::binary | ios::out | ios::trunc);

	const int emptyFileSize = this->BLOCKS_QUANTITY * sizeof(MemoryBlock) + this->FMDT_SIZE;
	byte setArray[emptyFileSize];
	SecureZeroMemory(setArray, emptyFileSize);
	this->sourceFile.write(setArray, emptyFileSize);

	this->sourceFile.close();
	this->sourceFile.open(this->FHYSICAL_SPACE_NAME, ios::binary | ios::in | ios::out | ios::ate);

	cout << endl << "all the data were reseted successfully" << endl << endl;
}

// добавление файла
void FileManager::AddFile()
{
	cout << endl << "enter new file name" << endl;
	string newFileName;
	cin >> newFileName;

	// проверка на существование файла
	for (int i = 0; i < this->FILES_QUANTITY; i++)
	{
		if (strcmp(this->FMDTField[i].fileName, newFileName.c_str()) == 0)
		{
			cout << endl << "this file is already exists" << endl;
			return;
		}
	}

	// добавление нового файла
	for (int i = 0; i < this->FILES_QUANTITY; i++)
	{
		// размещение файла в FMDT-поле
		if (this->FMDTField[i].initialOffset == 0)
		{
			this->FMDTField[i].initialOffset = this->FindFreeBlock(false);
			if (this->FMDTField[i].initialOffset == 0)
				break;

			int copySize = newFileName.length();
			CopyString(this->FMDTField[i].fileName, copySize, newFileName.c_str());

			this->SaveFileMetaDataTable();

			MemoryBlock tempBuffer;
			tempBuffer.busyFlag = 1;
			this->SaveMemoryBlock(tempBuffer, this->FMDTField[i].initialOffset);
			return;
		}
	}
}

// сохранение FMDT в исходном файле
void FileManager::SaveFileMetaDataTable()
{
	this->sourceFile.seekp(0); // позиция следующего символа для выходного потока
	this->sourceFile.write((byte*)this->FMDTField, sizeof(FileMetaDataTableField) * this->FILES_QUANTITY);
	this->sourceFile.flush(); // сихронизация буфера потока 
}

// запись блока с информацией в файл со смещением от начала
void FileManager::SaveMemoryBlock(MemoryBlock block, int offset)
{
	this->sourceFile.seekp(offset);
	this->sourceFile.write((byte*)&block, sizeof(MemoryBlock));
	this->sourceFile.clear();
	this->sourceFile.flush();
}

// очистка блока с установкой нулей
void FileManager::ClearMemoryBlock(MemoryBlock& block)
{
	block.nextOffset = 0;
	block.busyFlag = 0;
	SecureZeroMemory(block.fileData, this->BLOCK_DATA_SIZE);
}

// извлечение (чтение) блока из файла
FileManager::MemoryBlock FileManager::ReadMemoryBlock(int offset)
{
	MemoryBlock tempMemoryBlock;
	this->sourceFile.seekg(offset);
	this->sourceFile.read((byte*)&tempMemoryBlock, sizeof(MemoryBlock));
	this->sourceFile.clear();
	return tempMemoryBlock;
}

// поиск нового блока в памяти
// возвращает смещение блока от начала файла
int FileManager::FindFreeBlock(bool nextBlock)
{
	byte flag = 0;
	int offset = this->FIRST_BLOCK_OFFSET;
	for (int i = 0; i < this->BLOCKS_QUANTITY; i++)
	{
		this->sourceFile.seekg(offset);
		sourceFile.read(&flag, sizeof(byte));
		if (flag == 0)
		{
			if (nextBlock)
			{
				nextBlock = false;
				continue;
			}
			return offset; 
		}
		offset += sizeof(MemoryBlock);
	}
	return 0;
}

// копия строки с игнорированием '\0'
void FileManager::CopyString(char* destination, int destinationMaxSize, const char* source)
{
	for (int i = 0; i < destinationMaxSize && source[i] != '\0'; i++)
	{
		destination[i] = source[i];
	}
}

// удаление файла 
void FileManager::DeleteFile()
{
	cout << endl << "enter file name" << endl;
	cin.clear();
	string fileName;
	cin >> fileName;

	for (int i = 0; i < this->FILES_QUANTITY; i++)
	{
		if (strcmp(this->FMDTField[i].fileName, fileName.c_str()) == 0)
		{
			int offset = this->FMDTField[i].initialOffset;
			while (true)
			{
				MemoryBlock tempBuffer = this->ReadMemoryBlock(offset);
				int nextOffset = tempBuffer.nextOffset;
				this->ClearMemoryBlock(tempBuffer);
				this->SaveMemoryBlock(tempBuffer, offset);

				if (nextOffset == 0)
					break;
				else
					offset = nextOffset;
			}
			this->FMDTField[i].initialOffset = 0;
			SecureZeroMemory(this->FMDTField[i].fileName, this->FILENAME_LENGTH);
			this->SaveFileMetaDataTable();
			return;
		}
	}
	cout << endl << "this file doesn't exists" << endl;
}

// чтение файла в буферную строку
string FileManager::ReadFileToBufferString(int initialOffset)
{
	string bufferString;
	int offset = initialOffset;
	while (true)
	{
		MemoryBlock tempBuffer = this->ReadMemoryBlock(offset);
		char temp_buffer[this->BLOCK_DATA_SIZE + 1] = { 0 };
		this->CopyString(temp_buffer, this->BLOCK_DATA_SIZE, tempBuffer.fileData);
		bufferString.append(temp_buffer);
		if (tempBuffer.nextOffset == 0)
			return bufferString;
		else
			offset = tempBuffer.nextOffset;
	}
}

// открытие файла
void FileManager::OpenFile()
{
	cout << endl << "enter file name" << endl;
	string fileName;
	cin >> fileName;

	for (int i = 0; i < this->FILES_QUANTITY; i++)
	{
		if (strcmp(this->FMDTField[i].fileName, fileName.c_str()) == 0)
		{
			string fileBuffer = this->ReadFileToBufferString(this->FMDTField[i].initialOffset);
			string fileProcessing; // обработка файла
			fileProcessing = this->ProcessFileBuffer(fileBuffer); // содержимое файлов
			if (fileProcessing.length() == 0)
			{
				int offset = this->FMDTField[i].initialOffset;
				while (true)
				{
					MemoryBlock tempBuffer = this->ReadMemoryBlock(offset);
					int nextOffset = tempBuffer.nextOffset;
					this->ClearMemoryBlock(tempBuffer);
					this->SaveMemoryBlock(tempBuffer, offset);
					if (nextOffset == 0)
						break;
					else
						offset = nextOffset;
				}
				MemoryBlock firstFileBlock;
				firstFileBlock.busyFlag = 1;
				this->SaveMemoryBlock(firstFileBlock, this->FMDTField[i].initialOffset);
				return;
			}
			this->SaveFile(fileProcessing, this->FMDTField[i].initialOffset);
			return;
		}
	}
	cout << endl << "this file doesn't exists" << endl;
	return;
}

// сохранение файла 
void FileManager::SaveFile(string processedFile, int initialOffset)
{
	int unprocessedLength = processedFile.length();
	int offset = initialOffset;
	while (true)
	{
		int minSize = unprocessedLength < this->BLOCK_DATA_SIZE ? unprocessedLength : this->BLOCK_DATA_SIZE; // с проверкой на превышение
		MemoryBlock tempBuffer = this->ReadMemoryBlock(offset);
		SecureZeroMemory(tempBuffer.fileData, this->BLOCK_DATA_SIZE);
		tempBuffer.busyFlag = 1;

		this->CopyString(tempBuffer.fileData, minSize, processedFile.c_str());
		this->SaveMemoryBlock(tempBuffer, offset);
		unprocessedLength -= minSize;
		processedFile.erase(0, minSize); // удаление в диапазоне [0-minSize]

		// если все файлы были обработаны
		if (unprocessedLength == 0)
		{
			// если есть неиспользованные блоки после удаления, то чистим
			if (tempBuffer.nextOffset != 0)
			{
				int clearOffset = tempBuffer.nextOffset;
				tempBuffer.nextOffset = 0;
				this->SaveMemoryBlock(tempBuffer, offset);
				while (true)
				{
					MemoryBlock clearBlock = this->ReadMemoryBlock(clearOffset);
					int prevOffset = clearOffset;
					clearOffset = clearBlock.nextOffset;
					this->ClearMemoryBlock(clearBlock);
					this->SaveMemoryBlock(clearBlock, prevOffset);
					if (clearOffset == 0)
						break;
				}
			}
			else return;
		}

		// если блок был последним из тех, которые были выделены
		if (tempBuffer.nextOffset == 0)
		{
			// добавление новых блоков, если не все файлы были обработаны
			if (unprocessedLength != 0)
			{
				tempBuffer.nextOffset = this->FindFreeBlock(false);
				// если больше нет свободных блоков
				if (tempBuffer.nextOffset == 0)
				{
					cout << endl << "error... not enough memory" << endl;
					return;
				}
				this->SaveMemoryBlock(tempBuffer, offset);
			}
			else return;
		}
		offset = tempBuffer.nextOffset;
	}
}

// обработка файлов 
string FileManager::ProcessFileBuffer(string fileBuffer)
{
	string processingFile = fileBuffer;
	while (true)
	{
		system("cls");
		cout << processingFile << endl;
		cout << "------------------------------" << endl;
		cout << "1. add new information" << endl;
		cout << "2. delete certain information" << endl;
		cout << "3. delete all the information" << endl;
		cout << "0. close and save changes" << endl;
		cout << "------------------------------" << endl;

		switch (_getch())
		{
		case '1':
		{
			cout << "select action:" << endl;
			cout << "1. add new information" << endl;
			cout << "2. add new information from position" << endl;

			char command = 0;
			while (command != '1' && command != '2')
			{
				command = _getch();
			}

			string addInfo;
			cin.ignore();
			cin.clear();
			cout << "enter text:" << endl;
			getline(cin, addInfo);

			if (command == '1')
			{
				processingFile.append(addInfo);
			}
			else
			{
				int addingPos = -1;
				while (addingPos < 0 || (addingPos > processingFile.length() - 1))
				{
					cout << "enter the position:" << endl;
					cin >> addingPos;
				}
				processingFile.insert(addingPos, addInfo);
			}
			break;
		}
		case '2':
		{
			int deletePos = -1;
			int deleteSize = -1;
			while (deletePos < 0 || deleteSize < 0 || ((deleteSize + deletePos) > processingFile.length() + 1))
			{
				cout << "enter the position:" << endl;
				cin >> deletePos;

				cout << "enter the size:" << endl;
				cin >> deleteSize;
			}
			processingFile.erase(deletePos, deleteSize);
			break;
		}
		case '3':
		{
			processingFile.clear();
			break;
		}
		case '0':
		{
			return processingFile;
		}
		}
	}
}