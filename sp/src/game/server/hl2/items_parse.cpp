// ======================== Copyright Sixem Team, All rights reserved. ========================= //
// 
// Purpose: 
// 
// ============================================================================================= //

#include "cbase.h"
#include <KeyValues.h>
#include <tier0/mem.h>
#include "filesystem.h"
#include "utldict.h"

#include "items_parse.h"

#include "tier0/memdbgon.h"


static CUtlDict<FileItemInfo_t*, unsigned short> m_ItemsInfoDatabase;
static FileItemInfo_t gNullItemInfo;


//----------------------------------------------------------------------------------
// Purpose: 
//----------------------------------------------------------------------------------
static ITEM_FILE_INFO_HANDLE FindItemInfoSlot(const char *szName)
{
	unsigned short lookup = m_ItemsInfoDatabase.Find(szName);
	if (lookup != m_ItemsInfoDatabase.InvalidIndex())
	{
		return lookup;
	}

	FileItemInfo_t *insert = CreateItemInfo();

	lookup = m_ItemsInfoDatabase.Insert(szName, insert);
	Assert(lookup != m_ItemsInfoDatabase.InvalidIndex());
	return lookup;

}


//----------------------------------------------------------------------------------
// Purpose: 
//----------------------------------------------------------------------------------
ITEM_FILE_INFO_HANDLE LookupItemInfo(const char *name)
{
	return m_ItemsInfoDatabase.Find(name);
}


//----------------------------------------------------------------------------------
// Purpose: 
//----------------------------------------------------------------------------------
FileItemInfo_t* CreateItemInfo(void)
{
	return new FileItemInfo_t;
}


//----------------------------------------------------------------------------------
// Purpose: 
//----------------------------------------------------------------------------------
FileItemInfo_t* GetFileItemInfoFromHandle(ITEM_FILE_INFO_HANDLE handle)
{
	if (handle < 0 || handle >= m_ItemsInfoDatabase.Count())
	{
		return &gNullItemInfo;
	}

	if (handle == m_ItemsInfoDatabase.InvalidIndex())
	{
		return &gNullItemInfo;
	}

	return m_ItemsInfoDatabase[handle];
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
ITEM_FILE_INFO_HANDLE GetInvalidItemInfoHandle(void)
{
	return (ITEM_FILE_INFO_HANDLE)m_ItemsInfoDatabase.InvalidIndex();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void PrecacheFileItemInfoDatabase(IFileSystem *fileSystem, const unsigned char *pICEKey)
{
	if (m_ItemsInfoDatabase.Count())
		return;

	KeyValues *manifest = new KeyValues("itemscripts");
	if (manifest->LoadFromFile(fileSystem, "scripts/items_manifest.txt", "GAME"))
	{
		for (KeyValues *sub = manifest->GetFirstSubKey(); sub != NULL; sub = sub->GetNextKey())
		{
			if (!Q_stricmp(sub->GetName(), "file"))
			{
				char fileBase[512];
				Q_FileBase(sub->GetString(), fileBase, sizeof(fileBase));
				ITEM_FILE_INFO_HANDLE tmp;

				ReadItemDataFromFile(filesystem, fileBase, &tmp, pICEKey);
			}
			else
			{
				Error("Expecting 'file', got %s\n", sub->GetName());
			}
		}
	}
	manifest->deleteThis();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool ReadItemDataFromFile(IFileSystem* filesystem, const char *szItemName, ITEM_FILE_INFO_HANDLE *pHandle, const unsigned char *pICEKey)
{
	if (!pHandle)
	{
		Assert(0);
		return false;
	}

	*pHandle = FindItemInfoSlot(szItemName);
	FileItemInfo_t *pFileInfo = GetFileItemInfoFromHandle(*pHandle);
	Assert(pFileInfo);

	if (pFileInfo->bParsedScript)
		return true;

	char sz[128];
	Q_snprintf(sz, sizeof(sz), "scripts/items/%s", szItemName);

	KeyValues *pKV = ReadEncryptedKVFile(filesystem, sz, pICEKey, false);

	if (!pKV)
		return false;

	pFileInfo->Parse(pKV, szItemName);
	pKV->deleteThis();

	return true;
}


//------------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
FileItemInfo_t::FileItemInfo_t()
{
	bParsedScript = false;

	szClassName[0] = 0;
	szPrintName[0] = 0;

	szWorldModel[0] = 0;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void FileItemInfo_t::Parse(KeyValues *pKVData, const char *szItemName)
{
	bParsedScript = true;
	
	Q_strncpy(szClassName, szItemName, MAX_ITEM_STRING);
	Q_strncpy(szPrintName, pKVData->GetString("printname", "!!!Item missing"), MAX_ITEM_STRING);
	Q_strncpy(szWorldModel, pKVData->GetString("model"), MAX_ITEM_STRING);
}




