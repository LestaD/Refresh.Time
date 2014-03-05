#include "cbase.h"
#include "ai_basenpc.h"
#include "animation.h"
#include "player.h"			// For gEvilImpulse101 / CBasePlayer
#include "hl2_player.h"
#include "gamerules.h"		// For g_pGameRules
#include <KeyValues.h>
#include "baseviewmodel.h"
#include "in_buttons.h"
#include "soundent.h"
#include "items_parse.h"
#include "game.h"
#include "engine/IEngineSound.h"
#include "sendproxy.h"
#include "tier1/strtools.h"
#include "vphysics/constraints.h"
#include "npcevent.h"
#include "igamesystem.h"
#include "collisionutils.h"
#include "iservervehicle.h"
#include "func_break.h"
#include "items.h"
#include "invitems.h"



// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CItemList g_ItemList("CItemList");

//---------------------------------------------------------------------
// Purpose: CItemList
//---------------------------------------------------------------------

void CItemList::LevelShutdownPostEntity()
{
	m_list.Purge();
}

void CItemList::AddItem(CInvItem *pItem)
{
	m_list.AddToTail(pItem);
}

void CItemList::RemoveItem(CInvItem *pItem)
{
	m_list.FindAndRemove(pItem);
}



//---------------------------------------------------------------------
// Purpose: CInvItem
//---------------------------------------------------------------------

CInvItem::CInvItem(const char *entityName)
{
	OnInvItemCreated(this);
}

CInvItem::~CInvItem()
{
	OnInvItemDestroyed(this);
}

void CInvItem::ItemTouch(CBaseEntity *pOther)
{
	if (!pOther->IsPlayer())
		return;
	
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if (ItemCanBeTouchedByPlayer(pPlayer) == false)
		return;

	if (!MyTouch(pPlayer))
		return;

	BaseClass::ItemTouch(pOther);
}

bool CInvItem::MyTouch(CBasePlayer *pPlayer)
{
	CHL2_Player *pHL2PLayer = (CHL2_Player *)pPlayer;


	return true;
}


//---------------------------------------------------------------------
// Purpose: Add to list
//---------------------------------------------------------------------
void OnInvItemCreated(CInvItem *pItem)
{
	g_ItemList.AddItem(pItem);
}


//---------------------------------------------------------------------
// Purpose: Remove from list
//---------------------------------------------------------------------
void OnInvItemDestroyed(CInvItem *pItem)
{
	g_ItemList.AddItem(pItem);
}
