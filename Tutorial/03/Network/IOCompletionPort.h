//��ó: �����ߴ��� ���� '�¶��� ���Ӽ���'����
#pragma once

#include <thread>
#include <vector>

#include "Define.h"
#include "Session.h"
#include "../Util/delegateLib/Delegate.h"

class IOCompletionPort
{
public:
	IOCompletionPort(void);
	
	~IOCompletionPort(void);

	SA::delegate<void(Session)> OnConnect;
	SA::delegate<void(Session)> OnDisconnect;
	SA::delegate<void(Session, WSABUF*)> OnRecv;

	//������ �ʱ�ȭ�ϴ� �Լ�
	bool InitSocket();

	
	//------������ �Լ�-------//
	//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� 
	//������ ����ϴ� �Լ�
	bool BindandListen(int nBindPort);

	//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer(const UINT32 maxClientCount);

	//�����Ǿ��ִ� �����带 �ı��Ѵ�.
	void DestroyThread();

protected:
	void CreateClient(const UINT32 maxClientCount);

	//WaitingThread Queue���� ����� ��������� ����
	bool CreateWokerThread();
	
	//accept��û�� ó���ϴ� ������ ����
	bool CreateAccepterThread();

	//������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.
	Session* GetEmptyClientInfo();

	//CompletionPort��ü�� ���ϰ� CompletionKey��
	//�����Ű�� ������ �Ѵ�.
	bool BindIOCompletionPort(Session* pClientInfo);
  	
	//WSARecv Overlapped I/O �۾��� ��Ų��.
	bool BindRecv(Session* pClientInfo);

	//WSASend Overlapped I/O�۾��� ��Ų��.
	bool SendMsg(Session* pClientInfo, char* pMsg, int nLen);

	//Overlapped I/O�۾��� ���� �Ϸ� �뺸�� �޾� 
	//�׿� �ش��ϴ� ó���� �ϴ� �Լ�
	void WokerThread();

	//������� ������ �޴� ������
	virtual void AccepterThread();
	
	//������ ������ ���� ��Ų��.
	void CloseSocket(Session* pClientInfo, bool bIsForce = false);

	//Ŭ���̾�Ʈ ���� ���� ����ü
	std::vector<Session> mClientInfos;

	//Ŭ���̾�Ʈ�� ������ �ޱ����� ���� ����
	SOCKET		mListenSocket = INVALID_SOCKET;
	
	//���� �Ǿ��ִ� Ŭ���̾�Ʈ ��
	int			mClientCnt = 0;
	
	//IO Worker ������
	std::vector<std::thread> mIOWorkerThreads;

	//Accept ������
	std::thread	mAccepterThread;

	//CompletionPort��ü �ڵ�
	HANDLE		mIOCPHandle = INVALID_HANDLE_VALUE;
	
	//�۾� ������ ���� �÷���
	bool		mIsWorkerRun = true;

	//���� ������ ���� �÷���
	bool		mIsAccepterRun = true;
	//���� ����
	char		mSocketBuf[1024] = { 0, };
};