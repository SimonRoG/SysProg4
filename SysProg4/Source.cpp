#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include "Header.h"

using namespace std;

HWND hEdit;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
STARTUPINFO si1, si2;
PROCESS_INFORMATION pi1, pi2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const TCHAR CLASS_NAME[] = _T("Sample Window Class");
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hWnd = CreateWindowEx(0, CLASS_NAME, _T("Sample Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 565, 350, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return 0;
    }
    hEdit = CreateWindowEx(0, _T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 10, 10, 200, 20, hWnd, NULL, hInstance, NULL);
    HWND hButton1 = CreateWindowEx(0, _T("BUTTON"), _T("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 40, 200, 20, hWnd, (HMENU)1, hInstance, NULL);
    HWND hButton2 = CreateWindowEx(0, _T("BUTTON"), _T("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 70, 200, 20, hWnd, (HMENU)2, hInstance, NULL);
    HWND hButton3 = CreateWindowEx(0, _T("BUTTON"), _T("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 100, 200, 20, hWnd, (HMENU)3, hInstance, NULL);
    HWND hButton4 = CreateWindowEx(0, _T("BUTTON"), _T("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 130, 200, 20, hWnd, (HMENU)4, hInstance, NULL);

    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    SetConsoleOutputCP(1251);
    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case 1:
        {   
            ZeroMemory(&si1, sizeof(si1));
            ZeroMemory(&pi1, sizeof(pi1));
            ZeroMemory(&si2, sizeof(si2));
            ZeroMemory(&pi2, sizeof(pi2));
            si1.cb = sizeof(si1);
            si2.cb = sizeof(si2);

            if (!CreateProcess(L"C:\\Users\\Simon\\source\\repos\\SysProg2\\x64\\Debug\\SysProg2.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si1, &pi1)) {
                printf("Помилка створення першого процесу (%d)\n", GetLastError());
                break;
            }

            if (!CreateProcess(L"C:\\Users\\Simon\\source\\repos\\SysProg3\\x64\\Debug\\SysProg3.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si2, &pi2)) {
                printf("Помилка створення другого процесу (%d)\n", GetLastError());
                break;
            }
            HANDLE hPipe = CreateNamedPipe(L"\\\\.\\pipe\\MyNamedPipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);

            if (hPipe == INVALID_HANDLE_VALUE) {
                printf("Помилка створення іменованого каналу. Помилка: (%d)\n", GetLastError());
                break;
            }

            printf("Очікування підключення до іменованого каналу...\n");

            // Очікування підключення до іменованого каналу
            /*if (!ConnectNamedPipe(hPipe, NULL)) {
                printf("Помилка підключення до іменованого каналу. Помилка: (%d)\n", GetLastError());
                CloseHandle(hPipe);
                break;
            }*/

            printf("Успішно підключено до іменованого каналу.\n");

            // Створення файлу для обробки
            HANDLE hFile = CreateFile(L"C:\\FILE21\\file1.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                printf("Помилка створення файлу. Помилка: (%d)\n", GetLastError());
                CloseHandle(hPipe);
                break;
            }
            printf("Файл успішно створено.\n");

            CloseHandle(hFile);
            CloseHandle(hPipe);


            break;
        }
        case 2:
        {
            HANDLE hEventStart = CreateEvent(NULL, FALSE, FALSE, NULL);
            HANDLE hEventFinish = CreateEvent(NULL, FALSE, FALSE, NULL);

            ifstream file("C:\\FILE21\\FILE22\\input.txt");
            if (!file)
            {
                printf("Помилка відкриття файлу для обробки.\n");
                SetEvent(hEventFinish);
                break;
            }
            string line;
            while (getline(file, line))
            {
                printf("Обробка рядка: %s\n", line.c_str());
            }
            file.close();
            SetEvent(hEventFinish);

            SetEvent(hEventStart);
            WaitForSingleObject(hEventFinish, INFINITE);
            CloseHandle(hEventStart);
            CloseHandle(hEventFinish);
            break;
        }
        case 3:
        {
            int numThreads = 4;

            HANDLE hFirstChild = CreateThread(NULL, 0, FirstChildProcess, &numThreads, 0, NULL);
            if (hFirstChild == NULL) {
                printf("Помилка створення першого дочірнього процесу\n");
                break;
            }
            HANDLE hSecondChild = CreateThread(NULL, 0, SecondChildProcess, NULL, 0, NULL);
            if (hSecondChild == NULL) {
                printf("Помилка створення другого дочірнього процесу\n");
                break;
            }
            WaitForSingleObject(hFirstChild, INFINITE);
            WaitForSingleObject(hSecondChild, INFINITE);
            CloseHandle(hFirstChild);
            CloseHandle(hSecondChild);
            break;
        }
        case 4:
        {
            ifstream inputFile("C:\\FILE21\\input.txt");
            if (!inputFile.is_open()) {
                printf("Помилка відкриття файлу\n");
                break;
            }

            const int blockSize = 100;
            vector<string> blocks;
            string block;

            while (getline(inputFile, block)) {
                blocks.push_back(block);
                if (blocks.size() == blockSize) {
                    thread countThread([&blocks]() {
                        for (const string& block : blocks) {
                            int count = CountWordsStartingWithB(block);
                            printf("Кількість слів, що починаються з літери 'b': %d\n", count);
                        }});
                    countThread.join();
                    blocks.clear();
                }
            }

            if (!blocks.empty()) {
                thread countThread([&blocks]() {
                    for (const string& block : blocks) {
                        int count = CountWordsStartingWithB(block);
                        printf("Кількість слів, що починаються з літери 'b': %d\n", count);
                    }});
                countThread.join();
            }

            vector<string> wordsStartingWithB;
            inputFile.clear();
            inputFile.seekg(0, ios::beg);
            while (getline(inputFile, block)) {
                blocks.push_back(block);
                if (blocks.size() == blockSize) {
                    thread sortThread([&blocks, &wordsStartingWithB]() {
                        for (const string& block : blocks) {
                            stringstream ss(block);
                            string word;
                            while (ss >> word) {
                                if (!word.empty() && word[0] == 'b') {
                                    wordsStartingWithB.push_back(word);
                                }
                            }
                        }});
                    sortThread.join();
                    blocks.clear();
                }
            }
            if (!blocks.empty()) {
                thread sortThread([&blocks, &wordsStartingWithB]() {
                    for (const string& block : blocks) {
                        stringstream ss(block);
                        string word;
                        while (ss >> word) {
                            if (!word.empty() && word[0] == 'b') {
                                wordsStartingWithB.push_back(word);
                            }
                        }
                    }
                    });
                sortThread.join();
            }
            SortWordsAlphabetically(wordsStartingWithB);
            printf("Слова, що починаються з літери 'b' у алфавітному порядку:\n");
            for (const string& word : wordsStartingWithB) {
                printf("%s\n", word.c_str());
            }

            inputFile.close();
            break;
        }
        }
        break;
    }
    case WM_CLOSE: {
        WaitForSingleObject(pi1.hProcess, INFINITE);
        WaitForSingleObject(pi2.hProcess, INFINITE);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default: {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}