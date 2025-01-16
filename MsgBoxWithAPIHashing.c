
# Reference: https://trikkss.github.io/posts/hiding_windows_api_calls_part1/

#include <windows.h>
#include <stdio.h>
#include <winnt.h>

typedef int(WINAPI* messagebox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
void* CustomGetProcAddress(HMODULE dll_handle, unsigned long hashed_function_name);
unsigned long hash_djb2(unsigned char *str);

int main(void)
{
    //printf("%lu\n", hash_djb2("MessageBoxA"));
    //djb2 hash of MessageBoxA: 944706740
    unsigned long hash_function_name = 944706740;
    // getting handle of user32.dll
    HMODULE user32_dll = LoadLibraryA("C:\\Windows\\System32\\user32.dll");
    // parsing it to find the MessageBox function
    messagebox MsgBox_imported = (messagebox)CustomGetProcAddress(user32_dll, hash_function_name);
    // display the message box
    MsgBox_imported(NULL, "100 Days of red team", "100 days of red team",0);

    return 0;
}

void* CustomGetProcAddress(HMODULE dll_handle, unsigned long hashed_function_name) {
    IMAGE_DOS_HEADER* p_DOS_HDR  = (IMAGE_DOS_HEADER*) dll_handle; // convert your data into an IMAGE_DOS_HEADER* type defined in winnt.h
    IMAGE_NT_HEADERS* p_NT_HDR = (IMAGE_NT_HEADERS*) ((LPBYTE)dll_handle + p_DOS_HDR->e_lfanew ); // NT HEADERS are located at the raw offset defined in the e_lfanew header
    IMAGE_EXPORT_DIRECTORY* export_table = (IMAGE_EXPORT_DIRECTORY*)((LPBYTE)dll_handle + p_NT_HDR->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress); // .edata section

    DWORD numberOfNames = export_table->NumberOfNames;

    DWORD* functions_address = (DWORD*)((LPBYTE)dll_handle + export_table->AddressOfFunctions);
    DWORD* functions_names = (DWORD*)((LPBYTE)dll_handle + export_table->AddressOfNames);
    WORD*  functions_ordinal = (WORD*)((LPBYTE)dll_handle + export_table->AddressOfNameOrdinals);

    for(size_t i=0; i < numberOfNames; i++) {
        char *name = (char*)dll_handle + functions_names[i];
        if (hashed_function_name == hash_djb2(name)) {
            return (LPBYTE)dll_handle + functions_address[functions_ordinal[i]];
        }
    }

    return NULL;
}


unsigned long hash_djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
