#pragma once

#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <WS2tcpip.h>

#define MAX_SOCKBUF 1024	//패킷 크기
#define MAX_WORKERTHREAD 4  //쓰레드 풀에 넣을 쓰레드 수
#define BUFFERCOUNT 2
#define HEADERSIZE 20
#define BODYSIZE 1024

enum class IOOperation
{
	RECV,
	SEND
};

//WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx
{
	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O구조체
	SOCKET		m_socketClient;			//클라이언트 소켓
	WSABUF		m_wsaBuf;				//Overlapped I/O작업 버퍼
	char		m_szBuf[MAX_SOCKBUF]; //데이터 버퍼
	IOOperation m_eOperation;			//작업 동작 종류
};

//클라이언트 정보를 담기위한 구조체
struct stOverlappedSet
{
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O작업을 위한 변수

	stOverlappedSet()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
	}
};
