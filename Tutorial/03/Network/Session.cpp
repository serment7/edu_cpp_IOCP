#include "Session.h"

Session::Session()
	: MatchIndex(0), ConnectedSock(INVALID_SOCKET), IP(""), SessionStartTime("")
{
	OverlappedSet = {  };
}

void Session::SetConnectedSock(SOCKET InSock)
{
	ConnectedSock = InSock;
}

void Session::SetMatchIndex(int InMatchIndex)
{
	MatchIndex = InMatchIndex;
}

void Session::SetIp(const string& InIP)
{
	IP = InIP;
}

void Session::SetStartTime(const string& InTime)
{
	SessionStartTime = InTime;
}

const SOCKET& Session::GetConnectedSock() const
{
	return ConnectedSock;
}

const int& Session::GetMatchIndex() const
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
	return (*this) == INVALID_SESSION;
}

bool Session::operator==(const int& InMacroValue) const
{
	return MatchIndex == InMacroValue;
}
