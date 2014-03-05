
#ifndef INV_ITEMS_H
#define INV_ITEMS_H

#ifdef _WIN32
#pragma once
#endif

#include "items.h"

#define MAX_INVENTORY_ITEMS 100

class CInvItem : public CItem
{
public:
	DECLARE_CLASS(CInvItem, CItem);
	CInvItem( const char *entityName );
	~CInvItem();

	virtual void		ItemTouch(CBaseEntity *pOther);
	virtual bool		MyTouch(CBasePlayer *pPlayer);
};



class CItemList : public CAutoGameSystem
{
public:
	CItemList(char const *name) : CAutoGameSystem(name) {}
	virtual void LevelShutdownPostEntity();
	void AddItem(CInvItem *pItem);
	void RemoveItem(CInvItem *pItem);

	CUtlLinkedList<CInvItem*>m_list;
};

extern CItemList g_ItemList;

void OnInvItemCreated(CInvItem *pItem);
void OnInvItemDestroyed(CInvItem *pItem);

typedef CHandle<CInvItem> CInvItemHandle;

#endif // INV_ITEMS_H