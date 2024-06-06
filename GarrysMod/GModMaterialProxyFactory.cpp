#include "cbase.h"
#include "GModMaterialProxyFactory.h"
#include <materialsystem/imaterialsystem.h>

GarrysMod::MaterialProxyFactory* GarrysMod::MaterialProxyFactory::m_pSingleton = nullptr;
IMaterialProxyFactory* GarrysMod::MaterialProxyFactory::m_pBaseFactory = nullptr;

void GarrysMod::MaterialProxyFactory::Init()
{
	if (m_pSingleton != NULL)
		Error("MaterialProxyFactory isn't NULL!");

	m_pBaseFactory = materials->GetMaterialProxyFactory();

	if (m_pBaseFactory == NULL)
		Error("GetMaterialProxyFactory returned NULL!");

	m_pSingleton = new MaterialProxyFactory;
	materials->SetMaterialProxyFactory(m_pSingleton);
}

void GarrysMod::MaterialProxyFactory::Shutdown()
{
	materials->SetMaterialProxyFactory(m_pBaseFactory);
	m_pBaseFactory = NULL;
	delete m_pSingleton;
	m_pSingleton = NULL;
}

void GarrysMod::MaterialProxyFactory::Refresh()
{
	// Todo
	// materials-> [ecx + 104h]
}

IMaterialProxy* GarrysMod::MaterialProxyFactory::CreateProxy( const char *pName )
{
	IMaterialProxy* proxy = m_pBaseFactory->CreateProxy( pName );
	if ( proxy )
		return proxy;


}

void GarrysMod::MaterialProxyFactory::DeleteProxy( IMaterialProxy *pProxy )
{
	
}