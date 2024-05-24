#include <GarrysMod/IResources.h>
#include "CImageManager.h"
#include <string>
#include <vector>
#include <set>

class CImage : public IImage
{
public:
	virtual void RegenerateTextureBits( ITexture*, IVTFTexture*, Rect_t* );
	virtual void Release();

public:
	CImage( const std::string&, const std::string&, bool, bool );
	void OpenImage();
};

std::set<CImage*> Images;
std::vector<CImage*> DynamicImages;

CImage::CImage( const std::string& unknown1, const std::string& unknown2, bool unknown3, bool unknown4 )
{
	// ToDo
}

void CImage::OpenImage()
{
	// ToDo
}

void CImage::Release()
{
	// ToDo

	// FreeImage_Unload
}

void CImage::RegenerateTextureBits( ITexture* unknown1, IVTFTexture* unknown2, Rect_t* unknown3 )
{
	// ToDo
}

Color CImageManager::GetTextureColour( ITexture* texture, int x, int y )
{
	return Color(0, 0, 0); // ToDo
}

void CImageManager::SetImage( ITexture* texture, const char* unknown )
{

}

void CImageManager::LoadImage( const std::string&, const std::string&, bool, bool )
{

}

CImageManager imagemanager;