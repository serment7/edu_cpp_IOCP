#include "Server.h"
#include "Util\delegateLib\Delegate.h"

const UINT16 SERVER_PORT = 11021;
const UINT16 MAX_CLIENT = 100;		//총 접속할수 있는 클라이언트 수



int main()
{
	Server* server = new Server();

	server->Init(SERVER_PORT, MAX_CLIENT);

	server->Run();

	delete server;

	return 0;
}

