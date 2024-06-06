#include "materialsystem/imaterialproxyfactory.h"

namespace GarrysMod
{
	class MaterialProxyFactory : public IMaterialProxyFactory
	{
	public:
		virtual IMaterialProxy *CreateProxy( const char *proxyName );
		virtual void DeleteProxy( IMaterialProxy *pProxy );
	public:
		static void Init();
		static void Shutdown();
		static void Refresh();
	private:
		static MaterialProxyFactory* m_pSingleton;
		static IMaterialProxyFactory* m_pBaseFactory;
	};
}

