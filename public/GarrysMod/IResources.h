#pragma once

#include "tier1/interface.h"

#define INTERFACEVERSION_RESOURCES			"IResources001"

class ITexture;
class CUtlBuffer;
class Color;
class IMaterial;
class IVTFTexture;
struct Rect_t;

namespace Bootil
{
	class Buffer;
}

// ToDo: Check all return values
abstract_class IImage
{
public:
	virtual void RegenerateTextureBits( ITexture*, IVTFTexture*, Rect_t* ) = 0;
	virtual void Release() = 0;
};

abstract_class IVideoWriter
{
public:
	virtual void Start( const char*, const char*, const char*, int, int, int, const char*, int, float, bool ) = 0;
	virtual void AddFrame( int, int, float ) = 0;
	virtual void Finish() = 0;
	virtual void Delete() = 0;

	virtual void AddAudio( void*, uint, unsigned char, unsigned char ) = 0;
	virtual void StartMovie() = 0;
	virtual void EncodeRGB( void*, float ) = 0;
	virtual void EndMovie() = 0;

	virtual bool ManualFiling() = 0;

	virtual const char* FileExtension() = 0;
};

abstract_class IVideoHolly : IVideoWriter
{
public:
	virtual void AddAudio( void*, uint, unsigned char, unsigned char ) = 0;
	virtual void StartMovie() = 0;
	virtual void EncodeRGB( void*, float ) = 0;
	virtual void EndMovie() = 0;
	virtual bool ManualFiling() = 0;
	virtual const char* FileExtension() = 0;
};

abstract_class IResources
{
public:
	virtual int Init( CreateInterfaceFn ) = 0;
	virtual void Shutdown() = 0;
	virtual IVideoHolly* CreateMovie() = 0;
	virtual IMaterial* FindMaterial( const char*, const char*, bool, bool, bool ) = 0;
	virtual Color GetTextureColour( ITexture*, int, int ) = 0;
	virtual void SavePNG( int, int, unsigned char*, const char*, int, int ) = 0;
	virtual void SaveJPG( int, int, int, unsigned char*, const char*, int, int, CUtlBuffer* ) = 0;
	virtual bool ShouldRecordSound() = 0;
	virtual void AudioSamples( void*, uint, unsigned char, unsigned char ) = 0;
	virtual void SavePNGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int ) = 0;
	virtual void SaveJPGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int, int ) = 0;
	virtual void SetImage( ITexture*, const char* ) = 0;
};