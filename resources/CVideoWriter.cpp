#include "CVideoWriter.h"
#include <list>

std::list<CVideoWriter*> m_MovieList;

CVideoWriter::CVideoWriter()
{
	// ToDo
}

void CVideoWriter::Start( const char*, const char*, const char*, int, int, int, const char*, int, float, bool )
{
	// ToDo
}

void CVideoWriter::AddFrame( int, int, float )
{
	// ToDo
}

void CVideoWriter::Finish( )
{
	// ToDo
}

void CVideoWriter::Delete( )
{
	// ToDo
}

bool CVideoWriter::ManualFiling( )
{
	return false;
}

CVideoHolly::CVideoHolly()
{
	// ToDo
}

void CVideoHolly::AddAudio( void*, uint, unsigned char, unsigned char )
{
}

void CVideoHolly::StartMovie( )
{
	// ToDo
}

void CVideoHolly::EncodeRGB( void*, float )
{
}

void CVideoHolly::EndMovie( )
{
}

bool CVideoHolly::ManualFiling( )
{
	return true;
}

const char* CVideoHolly::FileExtension( )
{
	return ""; // ToDO
}