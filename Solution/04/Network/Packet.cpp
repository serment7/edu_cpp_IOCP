#include "Packet.h"

Packet::Packet()
{
	Buf[0].buf = new CHAR[HEADERSIZE];
	Buf[0].len = HEADERSIZE;
	Buf[1].buf = new CHAR[BODYSIZE];
	Buf[1].len = BODYSIZE;
}

Packet::Packet( const string& InHeader, const string& InBody )
{
	SetPacket( InHeader, InBody );
}

Packet::~Packet()
{
	if ( Buf[0].buf != nullptr )
	{
		delete[] Buf[0].buf;
	}

	if ( Buf[1].buf != nullptr )
	{
		delete[] Buf[1].buf;
	}
}

bool Packet::SetPacket( const string& InHeader, const string& InBody )
{
	return SetHeader( InHeader ) && SetBody( InBody );
}

bool Packet::SetHeader( const string& InHeader )
{
	if ( InHeader.length() > HEADERSIZE )
	{
		printf( "Error(SetHeader) : InHeader 값이 HEADERSIZE 초과" );
		return false;
	}

	return true;
}

bool Packet::SetBody( const string& InBody )
{
	if ( InBody.length() > BODYSIZE )
	{
		printf( "Error(SetBody) : InHeader 값이 BODYSIZE 초과" );
		return false;
	}

	return true;
}

string Packet::GetHeader()
{
	return Buf[0].buf;
}

string Packet::GetBody()
{
	return Buf[1].buf;
}
