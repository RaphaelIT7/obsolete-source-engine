#include "materialsystem/itexture.h"
#include "Color.h"
#include <string>

class CImageManager
{
public:
	Color GetTextureColour( ITexture* texture, int x, int y );
	void SetImage( ITexture* texture, const char* unknown );
	void LoadImage( const std::string&, const std::string&, bool, bool );
};

extern CImageManager imagemanager;