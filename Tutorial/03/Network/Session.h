#pragma once

#include <string>
#include <WinSock2.h>
#include "Define.h"

using namespace std;

#define INVALID_SESSION -1

class Session
{
public:
	Session();

	void SetConnectedSock(SOCKET InSock);
	void SetMatchIndex(int InMatchIndex);
	void SetIp(const string& InIP);
	void SetStartTime(const string& InTime);

	const SOCKET& GetConnectedSock() const;
	//Network�� Session List�� ��Ī�Ǵ� Index �� ��ȯ
	const int& GetMatchIndex() const;
	const string& GetIPStr() const;
	const string& GetStartTimeStr() const;
	stOverlappedEx& GetRecvOverlapped();
	stOverlappedEx& GetSendOverlapped();
	
	bool IsValid() const;
	bool operator==(const int& InMacroValue) const;

	void Recv();
	void Send();

private:
	int MatchIndex = INVALID_SESSION;
	SOCKET ConnectedSock = INVALID_SOCKET;
	string IP = "";
	string SessionStartTime = "";
	stOverlappedSet OverlappedSet;
};

