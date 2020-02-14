#include "Session.h"

Session::Session()
	: MatchIndex( 0 ), ConnectedSock( INVALID_SOCKET ), IP( "" ), SessionStartTime( "" )
{
	OverlappedSet = {};
}

void Session::SetConnectedSock( SOCKET InSock )
{
	ConnectedSock = InSock;
}

void Session::SetMatchIndex( int InMatchIndex )
{
	MatchIndex = InMatchIndex;
}

void Session::SetIp( const string& InIP )
{
	IP = InIP;
}

void Session::SetStartTime( const string& InTime )
{
	SessionStartTime = InTime;
}

const SOCKET& Session::GetConnectedSock() const
{
	return ConnectedSock;
}

int Session::GetMatchIndex() const
{
	return MatchIndex;
}

const string& Session::GetIPStr() const
{
	return IP;
}

const string& Session::GetStartTimeStr() const
{
	return SessionStartTime;
}

stOverlappedEx& Session::GetRecvOverlapped()
{
	return OverlappedSet.m_stRecvOverlappedEx;
}

stOverlappedEx& Session::GetSendOverlapped()
{
	return OverlappedSet.m_stSendOverlappedEx;
}

bool Session::IsValid() const
{
	return ( *this ) == INVALID_SESSION;
}

bool Session::operator==( const int& InMacroValue ) const
{
	return MatchIndex == InMacroValue;
}

bool Session::BindRecv()
{
	if ( IsValid() == false )
	{
		printf( "BindRecv : ��ȿ���� ���� ���Ͽ� �����߽��ϴ�.\n" );
		return false;
	}

	DWORD Flag = 0;
	DWORD NumberOfBytesRecvd;
	WSABUF RecvBuf;
	RecvBuf.buf = new char[MAX_SOCKBUF];
	bool RecvRet = WSARecv( ConnectedSock, &RecvBuf, MAX_SOCKBUF, &NumberOfBytesRecvd, &Flag, (LPWSAOVERLAPPED) &OverlappedSet.m_stRecvOverlappedEx, NULL );

	if ( RecvRet == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING )
	{
		printf( "Error(BindRecv) : Session ���� ����" );
		return false;
	}

	return true;
}

bool Session::SendMsg( const UINT32 InDataSize, WSABUF* InData )
{
	if ( IsValid() == false )
	{
		printf( "SendMsg : ��ȿ���� ���� ���Ͽ� �����߽��ϴ�.\n" );
		return false;
	}

	auto& var = GetSendOverlapped();
	
	DWORD NumberOfBytesSent = 0;
	bool SendRet = WSASend( ConnectedSock, InData, 1, &NumberOfBytesSent, 0, (LPWSAOVERLAPPED) &OverlappedSet.m_stSendOverlappedEx, NULL );

	if ( SendRet == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING )
	{
		printf( "Error(SendMsg) : Session ���� ����" );
		return false;
	}

	return true;
}
