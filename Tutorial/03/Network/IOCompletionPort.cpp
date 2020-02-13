#include "IOCompletionPort.h"

IOCompletionPort::IOCompletionPort(void) {}

IOCompletionPort::~IOCompletionPort(void)
{
	//������ ����� ������.
	WSACleanup();
}

bool IOCompletionPort::InitSocket()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet)
	{
		printf("[����] WSAStartup()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	//���������� TCP , Overlapped I/O ������ ����
	mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == mListenSocket)
	{
		printf("[����] socket()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	printf("���� �ʱ�ȭ ����\n");
	return true;
}

//------������ �Լ�-------//
//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� 
//������ ����ϴ� �Լ�

bool IOCompletionPort::BindandListen(int nBindPort)
{
	SOCKADDR_IN		stServerAddr;
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(nBindPort); //���� ��Ʈ�� �����Ѵ�.		
											  //� �ּҿ��� ������ �����̶� �޾Ƶ��̰ڴ�.
											  //���� ������� �̷��� �����Ѵ�. ���� �� �����ǿ����� ������ �ް� �ʹٸ�
											  //�� �ּҸ� inet_addr�Լ��� �̿��� ������ �ȴ�.
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//������ ������ ���� �ּ� ������ cIOCompletionPort ������ �����Ѵ�.
	int nRet = bind(mListenSocket, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	if (0 != nRet)
	{
		printf("[����] bind()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	//���� ��û�� �޾Ƶ��̱� ���� cIOCompletionPort������ ����ϰ� 
	//���Ӵ��ť�� 5���� ���� �Ѵ�.
	nRet = listen(mListenSocket, 5);
	if (0 != nRet)
	{
		printf("[����] listen()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	printf("���� ��� ����..\n");
	return true;
}

//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�

bool IOCompletionPort::StartServer(const UINT32 maxClientCount)
{
	CreateClient(maxClientCount);

	//CompletionPort��ü ���� ��û�� �Ѵ�.
	mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
	if (NULL == mIOCPHandle)
	{
		printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
		return false;
	}

	//���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	bool bRet = CreateWokerThread();
	if (false == bRet) {
		return false;
	}

	bRet = CreateAccepterThread();
	if (false == bRet) {
		return false;
	}

	printf("���� ����\n");
	return true;
}

//�����Ǿ��ִ� �����带 �ı��Ѵ�.

void IOCompletionPort::DestroyThread()
{
	mIsWorkerRun = false;
	CloseHandle(mIOCPHandle);

	for (auto& th : mIOWorkerThreads)
	{
		if (th.joinable())
		{
			th.join();
		}
	}

	//Accepter �����带 �����Ѵ�.
	mIsAccepterRun = false;
	closesocket(mListenSocket);

	if (mAccepterThread.joinable())
	{
		mAccepterThread.join();
	}
}

void IOCompletionPort::CreateClient(const UINT32 maxClientCount)
{
	for (UINT32 i = 0; i < maxClientCount; ++i)
	{
		mClientInfos.emplace_back();
	}
}

//WaitingThread Queue���� ����� ��������� ����

bool IOCompletionPort::CreateWokerThread()
{
	unsigned int uiThreadId = 0;
	//WaingThread Queue�� ��� ���·� ���� ������� ���� ����Ǵ� ���� : (cpu���� * 2) + 1 
	for (int i = 0; i < MAX_WORKERTHREAD; i++)
	{
		mIOWorkerThreads.emplace_back([this]() { WokerThread(); });
	}

	printf("WokerThread ����..\n");
	return true;
}

//accept��û�� ó���ϴ� ������ ����

bool IOCompletionPort::CreateAccepterThread()
{
	mAccepterThread = std::thread([this]() { AccepterThread(); });

	printf("AccepterThread ����..\n");
	return true;
}

//������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.

Session* IOCompletionPort::GetEmptyClientInfo()
{
	for (auto& client : mClientInfos)
	{
		if (client.IsValid() == false)
		{
			return &client;
		}
	}

	return nullptr;
}

//CompletionPort��ü�� ���ϰ� CompletionKey��
//�����Ű�� ������ �Ѵ�.

bool IOCompletionPort::BindIOCompletionPort(Session* pClientInfo)
{
	//socket�� pClientInfo�� CompletionPort��ü�� �����Ų��.
	auto hIOCP = CreateIoCompletionPort((HANDLE)pClientInfo->GetConnectedSock()
		, mIOCPHandle
		, (ULONG_PTR)(pClientInfo), 0);

	if (NULL == hIOCP || mIOCPHandle != hIOCP)
	{
		printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
		return false;
	}

	return true;
}

//WSARecv Overlapped I/O �۾��� ��Ų��.

bool IOCompletionPort::BindRecv(Session* pClientInfo)
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;

	//Overlapped I/O�� ���� �� ������ ������ �ش�.

	auto& RecvOverlapped = pClientInfo->GetRecvOverlapped();
	const auto& ClientSock = pClientInfo->GetConnectedSock();

	RecvOverlapped.m_wsaBuf.len = MAX_SOCKBUF;
	RecvOverlapped.m_wsaBuf.buf = RecvOverlapped.m_szBuf;
	RecvOverlapped.m_eOperation = IOOperation::RECV;

	int nRet = WSARecv(ClientSock,
		&(RecvOverlapped.m_wsaBuf),
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (RecvOverlapped),
		NULL);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[����] WSARecv()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

//WSASend Overlapped I/O�۾��� ��Ų��.

bool IOCompletionPort::SendMsg(Session* pClientInfo, char* pMsg, int nLen)
{
	DWORD dwRecvNumBytes = 0;
	auto& SendOverlapped = pClientInfo->GetSendOverlapped();
	const auto& ClientSock = pClientInfo->GetConnectedSock();

	//���۵� �޼����� ����
	CopyMemory(SendOverlapped.m_szBuf, pMsg, nLen);


	//Overlapped I/O�� ���� �� ������ ������ �ش�.
	SendOverlapped.m_wsaBuf.len = nLen;
	SendOverlapped.m_wsaBuf.buf = SendOverlapped.m_szBuf;
	SendOverlapped.m_eOperation = IOOperation::SEND;

	int nRet = WSASend(ClientSock,
		&(SendOverlapped.m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED) & (SendOverlapped),
		NULL);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[����] WSASend()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

//Overlapped I/O�۾��� ���� �Ϸ� �뺸�� �޾� 
//�׿� �ش��ϴ� ó���� �ϴ� �Լ�

void IOCompletionPort::WokerThread()
{
	//CompletionKey�� ���� ������ ����
	Session* pClientInfo = NULL;
	//�Լ� ȣ�� ���� ����
	BOOL bSuccess = TRUE;
	//Overlapped I/O�۾����� ���۵� ������ ũ��
	DWORD dwIoSize = 0;
	//I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������
	LPOVERLAPPED lpOverlapped = NULL;

	while (mIsWorkerRun)
	{
		//////////////////////////////////////////////////////
		//�� �Լ��� ���� ��������� WaitingThread Queue��
		//��� ���·� ���� �ȴ�.
		//�Ϸ�� Overlapped I/O�۾��� �߻��ϸ� IOCP Queue����
		//�Ϸ�� �۾��� ������ �� ó���� �Ѵ�.
		//�׸��� PostQueuedCompletionStatus()�Լ������� �����
		//�޼����� �����Ǹ� �����带 �����Ѵ�.
		//////////////////////////////////////////////////////
		bSuccess = GetQueuedCompletionStatus(mIOCPHandle,
			&dwIoSize,					// ������ ���۵� ����Ʈ
			(PULONG_PTR)&pClientInfo,		// CompletionKey
			&lpOverlapped,				// Overlapped IO ��ü
			INFINITE);					// ����� �ð�

										//����� ������ ���� �޼��� ó��..
		if (TRUE == bSuccess && 0 == dwIoSize && NULL == lpOverlapped)
		{
			mIsWorkerRun = false;
			continue;
		}

		if (NULL == lpOverlapped)
		{
			continue;
		}

		//client�� ������ ��������..			
		if (FALSE == bSuccess || (0 == dwIoSize && TRUE == bSuccess))
		{
			OnDisconnect(*pClientInfo);
			CloseSocket(pClientInfo);
			continue;
		}


		stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

		//Overlapped I/O Recv�۾� ��� �� ó��
		if (IOOperation::RECV == pOverlappedEx->m_eOperation)
		{
			pOverlappedEx->m_szBuf[dwIoSize] = NULL;
			//printf("[����] bytes : %d , msg : %s\n", dwIoSize, pOverlappedEx->m_szBuf);

			//Ŭ���̾�Ʈ�� �޼����� �����Ѵ�.
			SendMsg(pClientInfo, pOverlappedEx->m_szBuf, dwIoSize);
			BindRecv(pClientInfo);

			OnRecv(*pClientInfo, &pOverlappedEx->m_wsaBuf);
		}
		//Overlapped I/O Send�۾� ��� �� ó��
		else if (IOOperation::SEND == pOverlappedEx->m_eOperation)
		{
			printf("[�۽�] bytes : %d , msg : %s\n", dwIoSize, pOverlappedEx->m_szBuf);
		}
		//���� ��Ȳ
		else
		{
			printf("socket(%d)���� ���ܻ�Ȳ\n", (int)pClientInfo->GetConnectedSock());
		}
	}
}

//������� ������ �޴� ������

void IOCompletionPort::AccepterThread()
{
	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);

	while (mIsAccepterRun)
	{
		//������ ���� ����ü�� �ε����� ���´�.
		Session* pClientInfo = GetEmptyClientInfo();
		if (NULL == pClientInfo)
		{
			printf("[����] Client Full\n");
			return;
		}

		//Ŭ���̾�Ʈ ���� ��û�� ���� ������ ��ٸ���.
		auto AcceptedSock = accept(mListenSocket, (SOCKADDR*)&stClientAddr, &nAddrLen);

		if (INVALID_SOCKET == AcceptedSock)
		{
			continue;
		}

		pClientInfo->SetConnectedSock(AcceptedSock);

		//I/O Completion Port��ü�� ������ �����Ų��.
		bool bRet = BindIOCompletionPort(pClientInfo);
		if (false == bRet)
		{
			return;
		}

		//Recv Overlapped I/O�۾��� ��û�� ���´�.
		bRet = BindRecv(pClientInfo);
		if (false == bRet)
		{
			return;
		}

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(stClientAddr.sin_addr), clientIP, 32 - 1);
		//printf("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)\n", clientIP, (int)pClientInfo->m_socketClient);			

		OnConnect(*pClientInfo);

		//Ŭ���̾�Ʈ ���� ����
		++mClientCnt;
	}
}

//������ ������ ���� ��Ų��.

void IOCompletionPort::CloseSocket(Session* pClientInfo, bool bIsForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER�� ����

										// bIsForce�� true�̸� SO_LINGER, timeout = 0���� �����Ͽ� ���� ���� ��Ų��. ���� : ������ �ս��� ������ ���� 
	if (true == bIsForce)
	{
		stLinger.l_onoff = 1;
	}

	auto& TargetSock = pClientInfo->GetConnectedSock();

	//socketClose������ ������ �ۼ����� ��� �ߴ� ��Ų��.
	shutdown(TargetSock, SD_BOTH);

	//���� �ɼ��� �����Ѵ�.
	setsockopt(TargetSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

	//���� ������ ���� ��Ų��. 
	closesocket(TargetSock);

	pClientInfo->SetConnectedSock(INVALID_SOCKET);
	pClientInfo->SetMatchIndex(INVALID_SESSION);
}
