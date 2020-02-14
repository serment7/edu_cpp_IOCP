#pragma once
#include <WinSock2.h>
#include <string>
#include "Define.h"

using namespace std;

class Packet
{
public:
	Packet();
	Packet( const string& InHeader, const string& InBody );
	~Packet();

	bool SetPacket(const string& InHeader, const string& InBody);
	bool SetHeader(const string& InHeader);
	bool SetBody(const string& InBody);

	string GetHeader();
	string GetBody();

private:
	WSABUF Buf[BUFFERCOUNT];
};