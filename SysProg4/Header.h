#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

DWORD WINAPI FirstChildProcess(LPVOID lpParam) {
    int numThreads = *(int*)lpParam;
    HANDLE hFile = CreateFile(L"C:\\FILE21\\file11.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ïîìèëêà ñòâîðåííÿ ôàéëó ¹1\n");
        return 1;
    }
    DWORD bytesWritten;
    WriteFile(hFile, &numThreads, sizeof(numThreads), &bytesWritten, NULL);
    CloseHandle(hFile);
    hFile = CreateFile(L"C:\\FILE21\\file11.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ïîìèëêà â³äêðèòòÿ ôàéëó ¹1\n");
        return 1;
    }
    const int bufferSize = 128;
    char buffer[bufferSize];
    DWORD bytesRead;
    while (ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
        // Êîä äëÿ ïåðåäà÷³ áëîê³â äðóãîìó ïîòîêó
    }
    CloseHandle(hFile);
    return 0;
}

DWORD WINAPI SecondChildProcess(LPVOID lpParam) {
    HANDLE hFile = CreateFile(L"C:\\FILE21\\file11.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ïîìèëêà â³äêðèòòÿ ôàéëó ¹1\n");
        return 1;
    }

    int numThreads;
    DWORD bytesRead;
    ReadFile(hFile, &numThreads, sizeof(numThreads), &bytesRead, NULL);
    CloseHandle(hFile);

    vector<string> results;
    for (int i = 0; i < numThreads; i++) {
        // ßêàñü îáðîáêà áëîêó àáî ðÿäêà
        string res = to_string(i) + "Ðåçóëüòàò îáðîáêè áëîêó/ðÿäêà\n";
        // Ðåçóëüòàò îáðîáêè
        results.push_back(res);
    }

    hFile = CreateFile(L"C:\\FILE21\\file22.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ïîìèëêà ñòâîðåííÿ ôàéëó ¹2\n");
        return 1;
    }
    for (const string& result : results) {
        DWORD bytesWritten;
        WriteFile(hFile, result.c_str(), result.length(), &bytesWritten, NULL);
    }
    CloseHandle(hFile);

    ifstream file("C:\\FILE21\\file22.txt");
    if (!file) {
        printf("Ïîìèëêà â³äêðèòòÿ ôàéëó\n");
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
