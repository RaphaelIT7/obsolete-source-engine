#include "cbase.h"
#include "GModMaterialProxy.h"
#include "toolframework_client.h"
#include "Lua/LuaHelper.h"
#include "Externals.h"
#include "vprof.h"

GarrysMod::MaterialProxy::MaterialProxy( std::string name )
{
	m_strName = name;
}

bool GarrysMod::MaterialProxy::Init( IMaterial* pMaterial, KeyValues* pKeyValues )
{
	m_pMaterial = pMaterial;

	// ToDo
	// Call matproxy.Init
	// Were gonna need LuaHelper::KeyValuesToTable for this.

	return true;
}

void GarrysMod::MaterialProxy::CallBind( C_BaseEntity* ent )
{
	VPROF( "MaterialProxy::CallBind", VPROF_BUDGETGROUP_GLUA );

	if (LuaHelper::PushFunction( g_Lua, "matproxy", "Call" ))
	{
		// I tried 
		g_Lua->PushString( m_strName.c_str() );
		Push_Material(m_pMaterial);
		Push_Entity(ent);

		if(g_Lua->CallFunctionProtected(3, 0, 0) == 0){
			
		}
	}
}

void GarrysMod::MaterialProxy::OnBind( void* magic )
{
	VPROF( "MaterialProxy::OnBind", VPROF_BUDGETGROUP_GLUA );

	// ToDo
	// CallBind()

	if ( ToolsEnabled() )
	{
		ToolFramework_RecordMaterialParams( GetMaterial() );
	}
}

void GarrysMod::MaterialProxy::Release()
{
	// ToDO
}

IMaterial* GarrysMod::MaterialProxy::GetMaterial()
{
	return m_pMaterial;
}