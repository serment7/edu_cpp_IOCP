#pragma once
#include "Network\Session.h"
#include <WinSock2.h>
#include <string>

using namespace std;

class Network;

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

protected:
	bool ProcessCommand(string InCommand);

private:
	Network* network = nullptr;
	bool RunState = false;
};

