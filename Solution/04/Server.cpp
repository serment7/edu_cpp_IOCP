#include "Server.h"
#include "Network\Network.h"
#include <sstream>
#include <iostream>
#include "ServerCommandDefine.h"

void Server::Init(int InBindPort, UINT16 InMaxClientCount)
{
	RunState = true;

	network = new Network();

	//소켓을 초기화
	network->InitSocket();

	//소켓과 서버 주소를 연결하고 등록 시킨다.
	network->BindandListen(InBindPort);

	network->StartServer(InMaxClientCount);

	network->BindConnectEvent(ConnectDelegate::create<Server, &Server::OnConnect>(this));
	network->OnDisconnect = SA::delegate<void(Session)>::create<Server, &Server::OnDisconnect>(this);
	network->OnRecv = SA::delegate<void(Session, WSABUF*)>::create<Server, &Server::OnRecv>(this);
}

void Server::Run()
{
	while (RunState == true)
	{
		printf("Server %");
		string InputCommand;
		getline(std::cin, InputCommand);
		ProcessCommand(InputCommand);

		RunState = false;
		network->DestroyThread();
	}
}

Server::~Server()
{
	if (RunState == true)
	{
		RunState = false;
		network->DestroyThread();
	}
}

void Server::OnConnect(Session InSession)
{
	printf("<클라이언트 연결>\n");
	printf("세션 정보: \n");
	printf("\t\t IP : %s\n", InSession.GetIPStr().c_str());
	printf("\t\t 소켓 핸들 : %d\n", (int)InSession.GetConnectedSock());
}

void Server::OnDisconnect(Session InSession)
{
	printf("<클라이언트 종료>\n");
	printf("세션 정보: \n");
	printf("\t\t IP : %s\n", InSession.GetIPStr().c_str());
	printf("\t\t 소켓 핸들 : %d\n", (int)InSession.GetConnectedSock());
}

void Server::OnRecv(Session InSession, WSABUF* WsaBuf)
{
	printf("<클라이언트 통신>\n");
	printf("통신 정보: \n");
	printf("\t\t IP : %s\n", InSession.GetIPStr().c_str());
	printf("\t\t 받은 패킷 : %s\n", WsaBuf->buf);
}

bool Server::ProcessCommand(string InCommand)
{
	stringstream ss(InCommand);
	string command;
	ss >> command;
	if (command == COMMAND_QUIT)
	{
		
	}
	else if (command == COMMAND_PING)
	{
		string target;
		ss >> target;

		//Send Ping
	}
	
	return false;
}
