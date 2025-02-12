#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;
// from utf-8 to ANSI
std::string utf8_to_ansi(const std::string& utf8_string) {
    int wchar_length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, NULL, 0);
    std::wstring wide_string(wchar_length, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &wide_string[0], wchar_length);

    int ansi_length = WideCharToMultiByte(CP_ACP, 0, wide_string.c_str(), -1, NULL, 0, NULL, NULL);
    std::string ansi_string(ansi_length, '\0');
    WideCharToMultiByte(CP_ACP, 0, wide_string.c_str(), -1, &ansi_string[0], ansi_length, NULL, NULL);

    return ansi_string;
}
