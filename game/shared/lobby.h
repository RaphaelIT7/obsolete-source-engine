//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:  GC based lobby.  Matchmaking assigns players to a lobby
//
//=============================================================================

#ifndef LOBBY_H
#define LOBBY_H
#ifdef _WIN32
#pragma once
#endif

#include "playergroup.h"

namespace GCSDK
{
class CSharedObject;

class ILobby : public IPlayerGroup
{
public:
	virtual ~ILobby() { }

};

class CLobbyInvite : public GCSDK::CProtoBufSharedObject<CSOLobbyInvite, k_EProtoObjectLobbyInvite>, public GCSDK::IPlayerGroupInvite
{
	//This is disabled since people shouldn't create these objects directly and should instead instantiate game specific versions of them
	//DECLARE_CLASS_MEMPOOL( CLobbyInvite );

public:
	const static int k_nTypeID = k_EProtoObjectLobbyInvite;

	virtual const CSteamID GetSenderID() const { return Obj().sender_id(); }
	virtual PlayerGroupID_t GetGroupID() const { return Obj().group_id(); }
	virtual const char* GetSenderName() const { return Obj().sender_name().c_str(); }

	virtual GCSDK::CSharedObject* GetSharedObject() { return this; }

};

}

#endif
