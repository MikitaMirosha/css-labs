#include <iostream>
#include <windows.h>
#include <chrono>
using namespace std;

int main()
{
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;
    ZeroMemory(&sInfo, sizeof(sInfo));
    // sInfo.cb = sizeof(sInfo);
    ZeroMemory(&pInfo, sizeof(pInfo));
    if (!CreateProcess(L"D:\\VSprojects\\lab1Child\\Release\\lab1Child.exe", NULL, NULL, NULL, false, 0, NULL, NULL, &sInfo, &pInfo))
    {
        cout << "Error! Process is not created..." << endl;
        return 1;
    }
    cout << "Parent process time:" << endl;
    cout << chrono::system_clock::to_time_t(chrono::system_clock::now()) << endl;
    WaitForSingleObject(pInfo.hProcess, INFINITE);
    CloseHandle(pInfo.hProcess);
    CloseHandle(pInfo.hThread);
    system("pause");
    return 0;
}
// STARTUPINFO - структура, которая определяет оконный терминал, рабочий стол, 
// стандартный дескриптор и внешний вид основного окна для нового процесса

// PROCESS_INFORMATION - структура, которая заполняется функцией CreateProcess 
// с информацией о недавно созданном процессе и его первичном потоке

// ZeroMemory заполняет блок памяти нулями

// cb - устанавливает размер структуры, в байтах

// CreateProcess создает новый процесс и его первичный (главный) поток

// WaitForSingleObject - ожидает завершение одной задачи или одного процесса
// INFINITE - ждет бесконечно, пока процесс не завершится
// hProcess - поле процесса, где процесс идентифицирован
// CloseHandle закрывает дескриптор открытого объекта

