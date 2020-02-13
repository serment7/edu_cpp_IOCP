#pragma once
#include "Network\Session.h"
#include <WinSock2.h>

class IOCompletionPort;

class Server
{
public:
	void Init(int, UINT16);
	void Run();

	~Server();
public:
	void OnConnect(Session);
	void OnDisconnect(Session);
	void OnRecv(Session, WSABUF*);

private:
	IOCompletionPort* network = nullptr;
	bool RunState = false;
};

