#include "GarrysMod/Lua/LuaObject.h"

#ifndef CLIENT_DLL
#include "recipientfilter.h"
#endif

struct LuaNetworkedEntity_t
{
	// ToDo
};

struct LuaNetworkedVar_t
{
	// ToDo
};

class CLuaNetworkedVars
{
public:
	CLuaNetworkedVars();
	
	void RegisterUserMessages();
#ifndef CLIENT_DLL
	void Cycle();
	void UpdateEntityVars( LuaNetworkedEntity_t& unknown1, CRecipientFilter& unknown2, bool unknown3 );
	void UpdateEntityVar(LuaNetworkedEntity_t& unknown1, LuaNetworkedVar_t& unknown2, float unknown3, CRecipientFilter&, bool unknown4 );
	void ClearEntity( EHANDLE& handle );
#endif
	LuaNetworkedVar_t& FindEntityVar( EHANDLE& handle, const char* var, bool unknown );
	void PushNetworkedVar( EHANDLE& handle, const char* var );
	void SetNetworkedVar( EHANDLE& handle, const char* var, GarrysMod::Lua::ILuaObject* obj );
	void SetNetworkedVarProxy( EHANDLE& handle, const char* var, GarrysMod::Lua::ILuaObject* obj );
#ifndef CLIENT_DLL
	void PlayerInsert( CBasePlayer* ply );
#endif
	void BuildNetworkVarTables(); // Probably returns an ILuaObject
	void BuildEntityNetworkVarTable( CBaseEntity* ent );

private:
	CUtlRBTree<CUtlMap<char const*, LuaNetworkedVar_t, unsigned short>::Node_t, unsigned short, CUtlMap<char const*, LuaNetworkedVar_t, unsigned short>::CKeyLess, CUtlMemory<UtlRBTreeNode_t<CUtlMap<char const*, LuaNetworkedVar_t, unsigned short>::Node_t, unsigned short>, unsigned short>> NetVars;
};