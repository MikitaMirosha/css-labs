#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

#define memoryBlocks 512 
#define entireMemory 32768  

int index;
int memorySize;
void* indexAmount[64];
unsigned int lastBlockBuffer;
char totalMemory[entireMemory];
char* blockBuffer[memoryBlocks];
char* activeBuffer[memoryBlocks];
unsigned int blockBufferCounter = 1;
unsigned int activeBlocksAmount = 0;
unsigned int blockSize[memoryBlocks];
unsigned int blockBufferSize[memoryBlocks];
unsigned long availableMemory = entireMemory;

void showMenu();
void showInfo();
void initialization();
int _free(void* memoryBlock);
void* _malloc(unsigned long size);
void* _realloc(void* memory_block, int size);

int main()
{
    setlocale(LC_ALL, "Russian");
    int command;
    initialization();
    while (true)
    {
        showMenu();
        cin >> command;
        switch (command)
        {
        case 1:
        {
            system("cls");
            int check;

            do 
            {
                cout << "Введите индекс: ";
                rewind(stdin);
                check = scanf_s("%d", &index);
            } while (check < 1 || !index);

            if (index > 64 || index < 1)
            {
                cout << "Ошибка 04: Неверный индекс." << endl;
                cout << "Введите индекс в диапазоне [1..64]" << endl;
                break;
            }

            if (indexAmount[index] == NULL)
            {
                do
                {
                    cout << "Введите размер: ";
                    rewind(stdin);
                    check = scanf_s("%d", &memorySize);
                } while (check < 1 || !memorySize);

                indexAmount[index] = _malloc(memorySize);
                if (indexAmount[index] == NULL)
                {
                    cout << "Ошибка 01: недостаточно памяти." << endl;
                    rewind(stdin);
                }
                else
                {
                    cout << "-----------------------------------" << endl;
                    cout << "Индекс: " << index << endl;
                    cout << "Занятых блоков: " << activeBlocksAmount << endl;
                    cout << "Выделено памяти: " << memorySize << endl;
                }
            }
            else 
            {
                cout << "Ошибка 05: данный блок уже используется." << endl;
                system("pause");
                break;
            }
            break;
        }
        case 2:
        {
            system("cls");
            int check;

            do
            {
                cout << "Введите индекс: ";
                rewind(stdin);
                check = scanf_s("%d", &index);
            } while (check < 1 || !index);

            if (index > 64 || index < 1)
            {
                cout << "Ошибка 04: Неверный индекс." << endl;
                cout << "Введите индекс в диапазоне [1..64]" << endl;
                break;
            }

            if (_free(indexAmount[index]) == -1)
            {
                cout << "Ошибка 02: блок не найден." << endl;
                cout << "Занятых блоков: " << activeBlocksAmount << endl;
                break;
            }
            cout << "Память освобождена" << endl;
            cout << "Занятых блоков: " << activeBlocksAmount << endl;
            break;
        }
        case 3:
        {
            system("cls");
            int check;

            do
            {
                cout << "Введите индекс: ";
                rewind(stdin);
                check = scanf_s("%d", &index);
            } while (check < 1 || !index);

            if (index > 64 || index < 1)
            {
                cout << "Ошибка 04: Неверный индекс." << endl;
                cout << "Введите индекс в диапазоне [1..64]" << endl;
                break;
            }

            if (indexAmount[index] == NULL)
            {
                cout << "Ошибка 02: блок не найден." << endl;
                break;
            }

            do
            {
                cout << "Введите размер: ";
                rewind(stdin);
                check = scanf_s("%d", &memorySize);
            } while (check < 1 || !memorySize);

            indexAmount[index] = _realloc(indexAmount[index], memorySize);
            cout << "Занятых блоков: " << activeBlocksAmount << endl;
            cout << "Память переопределена" << endl;
            break;
        }
        case 4:
        {
            showInfo();
            break;
        }
        case 5:
        {
            return 0;
        }
        default:
        {
            system("cls");
            break;
        }
        }
    }
}

void showMenu()
{
    cout << "-----------------------------------" << endl;
    cout << "Выберите команду:" << endl << endl;
    cout << "1. Выделить память (malloc)" << endl;
    cout << "2. Освободить память (free)" << endl;
    cout << "3. Переопределить память (realloc)" << endl;
    cout << "4. Показать информацию о памяти" << endl;
    cout << "5. Выход из программы" << endl;
    cout << "-----------------------------------" << endl;
}

void showInfo()
{
    system("cls");
    cout << "Информация о памяти" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Объем свободной памяти:" << endl;
    cout << availableMemory << endl;
    cout << "Количество занятых блоков:" << endl;
    cout << activeBlocksAmount << endl;
}

void initialization()
{
    blockBuffer[0] = totalMemory;
    blockBufferSize[0] = availableMemory;
}

void* _malloc(unsigned long size)
{
    char* tempBuffer = NULL;
    unsigned long int lastBlock;

    if (size > availableMemory)
    {
        cout << "Ошибка 03: превышен размер свободной памяти" << endl;
        rewind(stdin);
        return tempBuffer;
    }

    for (int i = 0; i < blockBufferCounter; i++)
    {
        if (size <= blockBufferSize[i])
        {
            tempBuffer = blockBuffer[i];
            lastBlock = i;
            break;
        }
    }

    if (!tempBuffer)
        return NULL;

    activeBuffer[activeBlocksAmount] = tempBuffer;
    blockSize[activeBlocksAmount] = size;
    activeBlocksAmount++;
    lastBlockBuffer++;
    blockBuffer[lastBlock] = (char*)(lastBlock + size + 1);
    blockBuffer[lastBlock] = blockBuffer[lastBlock] - size;
    availableMemory -= size;

    return tempBuffer;
}

int _free(void* memoryBlock)
{
    char* tempBuffer = NULL;
    unsigned int lastBlock;

    for (int i = 0; i < lastBlockBuffer; i++)
    {
        if (memoryBlock == activeBuffer[i])
        {
            tempBuffer = activeBuffer[i];
            lastBlock = i;
            break;
        }
    }

    if (!tempBuffer)
        return -1;

    activeBuffer[lastBlock] = 0;
    activeBlocksAmount--;
    blockBuffer[blockBufferCounter] = (char*)memoryBlock;
    blockBufferSize[blockBufferCounter] = blockSize[lastBlock];
    blockBufferCounter++;
    availableMemory += blockSize[lastBlock];

    return 0;
}

void* _realloc(void* index, int size)
{
    char* tempBuffer = 0;
    int i, lastBlock;

    for (i = 0; i < lastBlockBuffer; i++)
    {
        if (index == activeBuffer[i])
        {
            if (activeBuffer[i] == NULL)
                return _malloc(size);
        }
    }

    if (size == 0)
    {
        _free(index);
        return NULL;
    }

    for (i = 0; i < lastBlockBuffer; i++)
        if (index == activeBuffer[i])
        {
            tempBuffer = activeBuffer[i];
            lastBlock = i;
            break;
        }

    if (!tempBuffer)
        return NULL;

    _free(index);
    return _malloc(size);
}