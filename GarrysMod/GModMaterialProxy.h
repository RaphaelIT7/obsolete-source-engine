#include "materialsystem/imaterialproxy.h"
#include <string>

namespace GarrysMod
{
	class MaterialProxy : public IMaterialProxy
	{
	public:
		virtual bool Init( IMaterial* pMaterial, KeyValues *pKeyValues );
		virtual void OnBind( void * );
		virtual void Release();
		virtual IMaterial *GetMaterial();
	public:
		MaterialProxy( std::string name );
		void CallBind( C_BaseEntity* ent );
	private:
		std::string m_strName = "";
		IMaterial* m_pMaterial = NULL;
	};
}