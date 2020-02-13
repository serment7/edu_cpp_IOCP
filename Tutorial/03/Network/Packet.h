#pragma once
#include <WinSock2.h>
#include "Session.h"

class Packet
{
public:
	int SessionMatchIndex;
	WSABUF* data;
	int BufferCount;

	Packet()
	{
		SessionMatchIndex = INVALID_SESSION;
		data = nullptr;
		BufferCount = 0;
	}
};