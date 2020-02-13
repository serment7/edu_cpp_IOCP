#include "Server.h"
#include "Util\delegateLib\Delegate.h"

const UINT16 SERVER_PORT = 11021;
const UINT16 MAX_CLIENT = 100;		//�� �����Ҽ� �ִ� Ŭ���̾�Ʈ ��



int main()
{
	Server* server = new Server();

	server->Init(SERVER_PORT, MAX_CLIENT);

	server->Run();

	delete server;

	return 0;
}

