//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include <malloc.h>
#include <string.h>
#include "packed_entity.h"
#include "basetypes.h"
#include "changeframelist.h"
#include "dt_send.h"
#include "dt_send_eng.h"
#include "server_class.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


// -------------------------------------------------------------------------------------------------- //
// PackedEntity.
// -------------------------------------------------------------------------------------------------- //

PackedEntity::PackedEntity()
{
	m_pServerClass = nullptr;
	m_pClientClass = nullptr;

	m_nEntityIndex = -1;
	m_ReferenceCount = -1;

	m_nSnapshotCreationTick = 0;
	m_nShouldCheckCreationTick = 0;

	m_nNewPackedDataSize = 0;
	m_pNewPackedData = nullptr;
}

PackedEntity::~PackedEntity()
{
	if (m_pNewPackedData)
	{
		free(m_pNewPackedData);
	}
}

void PackedEntity::SetServerAndClientClass( ServerClass *pServerClass, ClientClass *pClientClass )
{
	m_pServerClass = pServerClass;
	m_pClientClass = pClientClass;
	if ( pServerClass )
	{
		Assert( pServerClass->m_pTable );
		SetShouldCheckCreationTick( pServerClass->m_pTable->HasPropsEncodedAgainstTickCount() );
	}
}