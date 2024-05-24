#include "GarrysMod/IResources.h"
#include "CImageManager.h"
#include "CVideoWriter.h"
#include "tier3/tier3.h"
#include "FreeImage.h"
#include "Color.h"

abstract_class CResources : public IResources
{
public:
	virtual int Init( CreateInterfaceFn );
	virtual void Shutdown();
	virtual IVideoHolly* CreateMovie();
	virtual IMaterial* FindMaterial( const char*, const char*, bool, bool, bool );
	virtual Color GetTextureColour( ITexture*, int, int );
	virtual void SavePNG( int, int, unsigned char*, const char*, int, int );
	virtual void SaveJPG( int, int, int, unsigned char*, const char*, int, int, CUtlBuffer* );
	virtual bool ShouldRecordSound();
	virtual void AudioSamples( void*, uint, unsigned char, unsigned char );
	virtual void SavePNGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int );
	virtual void SaveJPGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int, int );
	virtual void SetImage( ITexture*, const char* );
};

CResources resources;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CResources, IResources, INTERFACEVERSION_RESOURCES, resources);

int CResources::Init( CreateInterfaceFn fn )
{
	ConnectTier1Libraries( &fn, 1 );
	ConnectTier2Libraries( &fn, 1 );
	ConnectTier3Libraries( &fn, 1 );

	FreeImage_Initialise( 0 );

	return 1;
}

void CResources::Shutdown()
{
	FreeImage_DeInitialise();
}

IVideoHolly* CResources::CreateMovie()
{
	return (IVideoHolly*)new CVideoHolly; // ToDo
}

IMaterial* CResources::FindMaterial( const char* unknown1, const char* unknown2, bool unknown3, bool unknown4, bool unknown5 )
{
	// if file ends with .jpg or .png

	// call imagemanager.LoadImage

	return nullptr; // ToDo
}

Color CResources::GetTextureColour( ITexture* texture, int x, int y ) // Guessing x & y
{
	return imagemanager.GetTextureColour( texture, x, y );
}

void CResources::SavePNG( int, int, unsigned char*, const char*, int, int )
{
	// ToDo
}

void CResources::SaveJPG( int, int, int, unsigned char*, const char*, int, int, CUtlBuffer* )
{
	// ToDo
}

bool CResources::ShouldRecordSound()
{
	return false; // ToDo
}

void CResources::AudioSamples( void*, uint, unsigned char, unsigned char )
{
	// ToDo
}

void CResources::SavePNGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int )
{
	// ToDo
}

void CResources::SaveJPGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int, int )
{
	// ToDo
}

void CResources::SetImage( ITexture*, const char* )
{
	// ToDo
}