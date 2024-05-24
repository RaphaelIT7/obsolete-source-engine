#include "GarrysMod/IResources.h"

class CVideoWriter : public IVideoHolly
{
public: 
	virtual void Start( const char*, const char*, const char*, int, int, int, const char*, int, float, bool );
	virtual void AddFrame( int, int, float );
	virtual void Finish();
	virtual void Delete();
	virtual void AddAudio( void*, uint, unsigned char, unsigned char ) {};
	virtual void StartMovie() {};
	virtual void EncodeRGB( void*, float ) {};
	virtual void EndMovie() {};

	virtual bool ManualFiling();

	virtual const char* FileExtension() { return ""; };

public:
	CVideoWriter();
};

class CVideoHolly : public CVideoWriter
{
public: 
	virtual bool ManualFiling() override;
	virtual void AddAudio( void*, uint, unsigned char, unsigned char ) override;
	virtual void StartMovie() override;
	virtual void EncodeRGB( void*, float ) override;
	virtual void EndMovie() override;
	virtual const char* FileExtension() override;

public:
	CVideoHolly();
};