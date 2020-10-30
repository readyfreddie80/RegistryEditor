#pragma once
#include<windows.h>


/* 
///// Source: https://github.com/NTDLS/NSWFL/blob/master/NSWFL_Registry.Cpp

HKEY - a handle type
 The handle is returned by the RegCreateKeyEx or RegOpenKeyEx function, or it can be one of the following predefined keys:
	HKEY_CLASSES_ROOT
	HKEY_CURRENT_CONFIG
	HKEY_CURRENT_USER
	HKEY_LOCAL_MACHINE
	HKEY_USERS 
	......etc, I guess  
	!!!  If the key is not one of the predefined registry keys,
	call the RegCloseKey function after you have finished using the handle. !!!
LPCSTR 
 • LP  — Long Pointer 
 • C   – Constant 
 • STR – String 
*/


void DeleteRegistryValue(
	HKEY    hKeyRoot,
	LPCTSTR pszSubKey,
	LPCTSTR pszValue); 

void Set_BinaryRegistryValue(
	HKEY hKeyRoot,
	LPCTSTR pszSubKey,
	LPCTSTR pszValue,
	PVOID pData,
	DWORD dwSize); 

HKEY CreateRegistryKey(
	HKEY    hKeyRoot,
	LPCTSTR pszSubKey,
	bool    isVolatile = false); 

HKEY CreateRegKeyStructure(
	HKEY			hKey,
	const wchar_t* sPath); 