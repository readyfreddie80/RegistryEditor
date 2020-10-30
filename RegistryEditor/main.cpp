// RegistryEditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RegistryModifiers.h"

int main()
{
    std::cout << "Hello World!\n";

    const wchar_t *myKey = L"SOFTWARE\\myKey"; 
    const wchar_t* myVal = L"myVal";
    
    unsigned char myValData = 1; 
    
    HKEY hiveKey = HKEY_CURRENT_USER; 
    
    while (1) {
        CreateRegistryKey(hiveKey, myKey);

        Set_BinaryRegistryValue(hiveKey, myKey, myVal, (void*)&myValData, 1);

        DeleteRegistryValue(hiveKey, myKey, myVal);

        RegDeleteKey(hiveKey, myKey);
    }
}

