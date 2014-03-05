// ======================== Copyright Sixem Team, All rights reserved. ========================= //
// 
// Purpose: 
// 
// ============================================================================================= //


#ifndef ITEMS_PARSE_H
#define ITEMS_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"
#include <stdio.h>

class IFileSystem;
class CHudTexture;
class KeyValues;

typedef unsigned short ITEM_FILE_INFO_HANDLE;

#define MAX_ITEM_STRING 80
#define MAX_ITEM_PREFIX 16

class FileItemInfo_t
{
public:
	FileItemInfo_t();

	virtual void Parse(KeyValues *pKeyValuesData, const char *szItemName);

public:
	bool		bParsedScript;

	char		szClassName[MAX_ITEM_STRING];
	char		szPrintName[MAX_ITEM_STRING];

	char		szWorldModel[MAX_ITEM_STRING];
};


// The item parse function
bool ReadItemDataFromFile(IFileSystem* filesystem, const char *szItemName, ITEM_FILE_INFO_HANDLE *pHandle, const unsigned char *pICEKey = NULL);


// If item has been loaded for the specified class name, this function return it
ITEM_FILE_INFO_HANDLE LookupItemInfo(const char *name);

FileItemInfo_t* GetFileItemInfoFromHandle(ITEM_FILE_INFO_HANDLE handle);
ITEM_FILE_INFO_HANDLE GetInvalidItemInfoHandle(void);
void PrecacheFileItemInfoDatabase(IFileSystem *fileSystem, const unsigned char *pICEKey = NULL);

extern KeyValues* ReadEncryptedKVFile(IFileSystem *fileSystem, const char *szFilenameWithoutExtension, const unsigned char *pICEKey, bool bForceReadEncryptedFile);

FileItemInfo_t* CreateItemInfo(void);

#endif // ITEMS_PARSE_H