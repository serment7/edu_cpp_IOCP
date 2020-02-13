#include "Server.h"
#include "Network\IOCompletionPort.h"

void Server::Init(int InBindPort, UINT16 InMaxClientCount)
{
	RunState = true;

	network = new IOCompletionPort();

	//������ �ʱ�ȭ
	network->InitSocket();

	//���ϰ� ���� �ּҸ� �����ϰ� ��� ��Ų��.
	network->BindandListen(InBindPort);

	network->StartServer(InMaxClientCount);

	network->OnConnect = SA::delegate<void(Session)>::create<Server, &Server::OnConnect>(this);
	network->OnDisconnect = SA::delegate<void(Session)>::create<Server, &Server::OnDisconnect>(this);
	network->OnRecv = SA::delegate<void(Session, WSABUF*)>::create<Server, &Server::OnRecv>(this);
}

void Server::Run()
{
	if (RunState == true)
	{
		printf("�ƹ� Ű �Է� �� ����˴ϴ�.");
		getchar();

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
	printf("<Ŭ���̾�Ʈ ����>\n");
	printf("���� ����: \n");
	printf("\t\t IP : %s\n", InSession.GetIPStr().c_str());
	printf("\t\t ���� �ڵ� : %d\n", (int)InSession.GetConnectedSock());
}

void Server::OnDisconnect(Session InSession)
{
	printf("<Ŭ���̾�Ʈ ����>\n");
	printf("���� ����: \n");
	printf("\t\t IP : %s\n", InSession.GetIPStr().c_str());
	printf("\t\t ���� �ڵ� : %d\n", (int)InSession.GetConnectedSock());
}

void Server::OnRecv(Session InSession, WSABUF* WsaBuf)
{
	printf("<Ŭ���̾�Ʈ ���>\n");
	printf("��� ����: \n");
	printf("\t\t IP : %s\n", InSession.GetIPStr().c_str());
	printf("\t\t ���� ��Ŷ : %s\n", WsaBuf->buf);
}