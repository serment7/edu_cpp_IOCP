#pragma once
#include "IOCompletionPort.h"
#include "../Util/delegateLib/Delegate.h"
#include "Session.h"
#include <queue>

class Network :
	public IOCompletionPort
{
	
public:

	SA::delegate<void(Session)> OnConnect;
	SA::delegate<void(Session)> OnDisconnect;
	SA::delegate<void(Session, WSABUF*)> OnRecv;
};

