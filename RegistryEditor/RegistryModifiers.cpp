#include "RegistryModifiers.h"


/*!
 Deletes a value from a given subkey and root

\param hKeyRoot  A handle to an open registry key. 
\param pszSubKey The name of the registry subkey to be opened.
				 If the registry value to be removed
				 is in the key specified by hKeyRoot,
				 pszSubKey must be a pointer to an empty string or NULL.
				 (!!!)The pszSubKey parameter can be NULL 
				 only if hKeyRoot is one of the predefined keys. (!!!)
\param pszValue  The registry value to be removed. 
				 If this parameter is NULL or an empty string, 
				 the value set by the RegSetValueEx function is removed.
*/
void DeleteRegistryValue(
	HKEY    hKeyRoot, 
	LPCTSTR pszSubKey, 
	LPCTSTR pszValue) {

	HKEY hKey;

	RegOpenKeyEx(
		hKeyRoot, 
		pszSubKey, 
		0,             // The key is not a symlink 
		KEY_SET_VALUE, // The mask required to create, delete, or set a registry value
		&hKey		   // A pointer to a variable that receives a handle to the opened key
	);        

	RegDeleteValue(hKey, pszValue);

	RegCloseKey(hKey);

	return; 
}


/*!
 Set a REG_BINARY value 

\param hKeyRoot  A handle to an open registry key.
\param pszSubKey The name of the registry subkey to be opened.
				 If the registry value to be removed
				 is in the key specified by hKeyRoot,
				 pszSubKey must be a pointer to an empty string or NULL.
				 (!!!)The pszSubKey parameter can be NULL
				 only if hKeyRoot is one of the predefined keys. (!!!)
\param pszValue  The registry value to be set.
				 If a value with this name is not already present in the key, 
				 the function adds it to the key.
\param pData     The data to be stored (typedef void *PVOID)
\param dwSize	 The size of the information pointed to by the pData parameter, in bytes.
*/
void Set_BinaryRegistryValue(
	HKEY hKeyRoot, 
	LPCTSTR pszSubKey, 
	LPCTSTR pszValue, 
	PVOID pData, 
	DWORD dwSize) {
	
	HKEY hKey;
	
	RegOpenKeyEx(
		hKeyRoot,
		pszSubKey, 
		0,         
		KEY_WRITE, 
		&hKey
	);

	RegSetValueEx(
		hKey, 
		pszValue, 
		0,                    //This parameter is reserved and must be zero.
		REG_BINARY,			  //The type of data pointed to by the pData parameter
		(unsigned char*)pData, 
		dwSize);

	RegCloseKey(hKey);
}


/*!
Creates the specified registry key. If the key already exists, the function opens it.
Note that key names are not case sensitive.

\param hKeyRoot   A handle to an open registry key.
				  The calling process must have KEY_CREATE_SUB_KEY access to the key.
\param pszSubKey  The name of a subkey that this function opens or creates. 
				  This parameter cannot be NULL. (!!!)
\param isVolatile If flase, this key created is not volatile; 
				  this is the default. 
				  The information is stored in a file and is preserved when the system is restarted.
				  If true, the information is stored in memory and is not preserved when the corresponding registry hive is unloaded. 
\return			  A handle to the opened or created key
*/
HKEY CreateRegistryKey(
	HKEY    hKeyRoot,
	LPCTSTR pszSubKey,
	bool    isVolatile) {

	
	DWORD dwOption = REG_OPTION_NON_VOLATILE;

	if (isVolatile == true) {
		dwOption = REG_OPTION_VOLATILE;
	}

	//------------------------------------------------------------------------------
	
	// structure, contains the security information associated with an object
	SECURITY_DESCRIPTOR SD; 
	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);	
	SetSecurityDescriptorDacl(
		&SD, 
		true, 
		0, 
		false
	);
	
	SECURITY_ATTRIBUTES SA;
	SA.nLength              = sizeof(SA);
	SA.lpSecurityDescriptor = &SD;
	SA.bInheritHandle       = false;

	//------------------------------------------------------------------------------

	HKEY  hKey;
	DWORD dwFunc;

	RegCreateKeyEx(
		hKeyRoot,
		pszSubKey,
		0,						 //This parameter is reserved and must be zero.
		(LPTSTR)NULL,			 //The user-defined class type of this key.
		dwOption,				 // If volatile
		KEY_WRITE,				 
		&SA,					 // Determines whether the returned handle can be inherited (no) 
		&hKey,					 // The pointer to a variable that receives a handle to the opened or created key
		&dwFunc					 // The pointer to a variable that receives one of the disposition information
	);

	return hKey; 

}

/*!
Creates the row of keys 
\param hKey       A handle to an open registry key.
				  The calling process must have KEY_CREATE_SUB_KEY access to the key.
\param sPath      Path after handled key of the formats: 
							key1\key2\...\keyN
							key1/key2/.../keyN
\return			  A handle to the last opened or created key

*/
HKEY CreateRegKeyStructure(
	HKEY			hKey,
	const wchar_t * sPath) {

	wchar_t sDir[MAX_PATH];

	int iNameSz = (int)wcslen(sPath);
	int iPos = 0;
	for (iPos = 0; iPos < iNameSz; iPos++) {
		if (sPath[iPos] == '\\' || sPath[iPos] == '/') {
			sDir[iPos] = '\0';
			CreateRegistryKey(hKey, sDir);
		}
		sDir[iPos] = sPath[iPos];
	}

	sDir[iPos] = '\0';
	return CreateRegistryKey(hKey, sDir);
}

