#include "GarrysMod/Lua/LuaObject.h"
#include "CLuaClass.h"

#ifdef GAME_DLL
#include "recipientfilter.h"
#endif

struct LuaNetworkedVar_t // This is Gmod's current version
{
	CLuaObject m_pLuaValue; // Holds the last set Value
	CLuaObject m_pLuaProxy; // Holds the set NWProxy
	float m_flLastUpdate = -1.0f;
	int m_iNetworkStringID = -1;
};

struct LuaNetworkedEntity_t // ToDo: Get Gmod's current version
{
	EHANDLE m_pHandle; // This seems to be wrong. But why :/
	CUtlRBTree<CUtlMap<char const*, LuaNetworkedVar_t, unsigned short>::Node_t, unsigned short, CUtlMap<char const*, LuaNetworkedVar_t, unsigned short>::CKeyLess, CUtlMemory<UtlRBTreeNode_t<CUtlMap<char const*, LuaNetworkedVar_t, unsigned short>::Node_t, unsigned short>, unsigned short>> m_pVars;
};

class CLuaNetworkedVars
{
public:
	CLuaNetworkedVars();
	
	void RegisterUserMessages();
#ifndef CLIENT_DLL
	void Cycle();
	void UpdateEntityVars( LuaNetworkedEntity_t& unknown1, CRecipientFilter& unknown2, bool unknown3 );
	void UpdateEntityVar( LuaNetworkedEntity_t& unknown1, LuaNetworkedVar_t& unknown2, float unknown3, CRecipientFilter&, bool unknown4 );
	void ClearEntity( EHANDLE& handle );
#endif
	LuaNetworkedVar_t& FindEntityVar( EHANDLE& handle, const char* var, bool unknown );
	void PushNetworkedVar( EHANDLE& handle, const char* var );
	void SetNetworkedVar( EHANDLE& handle, const char* var, GarrysMod::Lua::ILuaObject* obj );
	void SetNetworkedVarProxy( EHANDLE& handle, const char* var, GarrysMod::Lua::ILuaObject* obj );
#ifndef CLIENT_DLL
	void PlayerInsert( CBasePlayer* ply );
#endif
	CLuaObject* BuildNetworkVarTables(); // Probably returns an ILuaObject
	CLuaObject* BuildEntityNetworkVarTable( CBaseEntity* ent );

private:
	LuaNetworkedEntity_t m_pEnts[MAX_EDICTS];
	static float fNextTime;
};