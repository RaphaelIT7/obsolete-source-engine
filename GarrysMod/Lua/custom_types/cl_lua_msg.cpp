#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/bitbuf.h"

LUA_FUNCTION_STATIC( bf_read_Reset )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	if ( msg )
	{
		msg->Reset();
	}

	return 0;
}

LUA_FUNCTION_STATIC( bf_read_ReadFloat )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	LUA->PushNumber( msg ? msg->ReadFloat() : 0 );

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadShort )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	LUA->PushNumber( msg ? msg->ReadShort() : 0 );

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadChar )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	LUA->PushNumber( msg ? msg->ReadChar() : 0 );

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadLong )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	LUA->PushNumber( msg ? msg->ReadLong() : 0 );

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadString )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	char* pStr = new char[255];
	if ( msg->ReadString( pStr, 255 ) )
	{
		LUA->PushString( pStr );
	} else {
		LUA->PushString( "" );
	}

	delete pStr;

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadEntity )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	if ( msg )
	{
		Push_Entity( cl_entitylist->GetBaseEntity( msg->ReadLong() ) );
	} else {
		LUA->PushNil();
	}

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadVector )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	if ( msg )
	{
		Vector* vec = new Vector;
		msg->ReadBitVec3Coord( *vec );
		Push_Vector( vec );
	} else {
		LUA->PushNil();
	}

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadAngle )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	if ( msg )
	{
		QAngle* ang = new QAngle;
		msg->ReadBitAngles( *ang );
		Push_Angle( ang );
	} else {
		LUA->PushNil();
	}

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadVectorNormal )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	if ( msg )
	{
		Vector* vec = new Vector;
		msg->ReadBitVec3Normal( *vec );
		Push_Vector( vec );
	} else {
		LUA->PushNil();
	}

	return 1;
}

LUA_FUNCTION_STATIC( bf_read_ReadBool )
{
	LUA->CheckType(1, Type::UserMsg);

	bf_read* msg = (bf_read*)LC_bf_read.Get( 1 );
	LUA->PushBool( msg ? msg->ReadOneBit() : false );

	return 1;
}

void Push_bf_read( const bf_read* msg )
{
	LC_bf_read.Push( (void*)msg );
}

void bf_read_Class()
{
	g_Lua->CreateMetaTableType( "bf_read", Type::UserMsg );
		g_Lua->PushCFunction( bf_read_Reset );
		g_Lua->SetField(-2, "Reset");

		g_Lua->PushCFunction( bf_read_ReadFloat );
		g_Lua->SetField(-2, "ReadFloat");

		g_Lua->PushCFunction( bf_read_ReadShort );
		g_Lua->SetField(-2, "ReadShort");

		g_Lua->PushCFunction( bf_read_ReadChar );
		g_Lua->SetField(-2, "ReadChar");

		g_Lua->PushCFunction( bf_read_ReadLong );
		g_Lua->SetField(-2, "ReadLong");

		g_Lua->PushCFunction( bf_read_ReadString );
		g_Lua->SetField(-2, "ReadString");

		g_Lua->PushCFunction( bf_read_ReadEntity );
		g_Lua->SetField(-2, "ReadEntity");

		g_Lua->PushCFunction( bf_read_ReadVector );
		g_Lua->SetField(-2, "ReadVector");

		g_Lua->PushCFunction( bf_read_ReadAngle );
		g_Lua->SetField(-2, "ReadAngle");

		g_Lua->PushCFunction( bf_read_ReadVectorNormal );
		g_Lua->SetField(-2, "ReadVectorNormal");

		g_Lua->PushCFunction( bf_read_ReadBool );
		g_Lua->SetField(-2, "ReadBool");
}

CLuaClass LC_bf_read( "bf_read", Type::UserMsg, bf_read_Class );