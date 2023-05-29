#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

DWORD WINAPI FirstChildProcess(LPVOID lpParam) {
    int numThreads = *(int*)lpParam;
    HANDLE hFile = CreateFile(L"C:\\FILE21\\file11.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Помилка створення файлу №1\n");
        return 1;
    }
    DWORD bytesWritten;
    WriteFile(hFile, &numThreads, sizeof(numThreads), &bytesWritten, NULL);
    CloseHandle(hFile);
    hFile = CreateFile(L"C:\\FILE21\\file11.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Помилка відкриття файлу №1\n");
        return 1;
    }
    const int bufferSize = 128;
    char buffer[bufferSize];
    DWORD bytesRead;
    while (ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
        // Код для передачі блоків другому потоку
    }
    CloseHandle(hFile);
    return 0;
}

DWORD WINAPI SecondChildProcess(LPVOID lpParam) {
    HANDLE hFile = CreateFile(L"C:\\FILE21\\file11.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Помилка відкриття файлу №1\n");
        return 1;
    }

    int numThreads;
    DWORD bytesRead;
    ReadFile(hFile, &numThreads, sizeof(numThreads), &bytesRead, NULL);
    CloseHandle(hFile);

    vector<string> results;
    for (int i = 0; i < numThreads; i++) {
        // Якась обробка блоку або рядка
        string res = to_string(i) + "Результат обробки блоку/рядка\n";
        // Результат обробки
        results.push_back(res);
    }

    hFile = CreateFile(L"C:\\FILE21\\file22.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Помилка створення файлу №2\n");
        return 1;
    }
    for (const string& result : results) {
        DWORD bytesWritten;
        WriteFile(hFile, result.c_str(), result.length(), &bytesWritten, NULL);
    }
    CloseHandle(hFile);

    ifstream file("C:\\FILE21\\file22.txt");
    if (!file) {
        printf("Помилка відкриття файлу\n");
        return 1;
    }
    string line;
    while (getline(file, line)) {
        printf("%s\n", line.c_str());
    }
    file.close();
    return 0;
}


int CountWordsStartingWithB(const string& block) {
    int count = 0;
    string word;
    stringstream ss(block);
    while (ss >> word) {
        if (!word.empty() && word[0] == 'b') {
            count++;
        }
    }

    return count;
}

void SortWordsAlphabetically(vector<string>& words) {
    sort(words.begin(), words.end());
}