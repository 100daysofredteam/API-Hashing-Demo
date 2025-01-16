#include <windows.h>
#include <stdio.h>

typedef int(WINAPI* messagebox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

int main(void)
{
    // getting handle of user32.dll
    HMODULE user32_dll = LoadLibraryA("C:\\Windows\\System32\\user32.dll");
    // parsing it to find the MessageBox function
    messagebox MsgBox_imported = (messagebox)GetProcAddress(user32_dll, "MessageBoxA");
    // display the message box
    MsgBox_imported(NULL, "100 Days of red team", "100 days of red team",0);
    return 0;
}
