#pragma once
#include "IOCompletionPort.h"
#include "../Util/delegateLib/Delegate.h"
#include "Session.h"
#include "../Util/delegateLib/Delegate.h"
#include "Packet.h"
#include <queue>

using namespace std;

using ConnectDelegate = SA::delegate<void(Session)>;
using DisconnectDelegate = SA::delegate<void(Session)>;
using RecvDelegate = SA::delegate<void(Session)>;
using SendDelegate = SA::delegate<void(Session)>;

class Network : public IOCompletionPort
{
public:
	// IOCompletionPort을(를) 통해 상속됨
	virtual void OnConnect( const UINT32 InSessionIndex ) override;
	virtual void OnDisconnect( const UINT32 InSessionIndex ) override;
	virtual void OnRecv( const UINT32 InSessionIndex, const WSABUF* InData ) override;
	virtual void OnSend( const UINT32 InSessionIndex ) override;

	void BindConnectEvent(ConnectDelegate InDelegate);
	void BindDisconnectEvenet( DisconnectDelegate InDelegate );
	void BindRecvEvent( RecvDelegate InDelegate );
	void BindSendEvent( SendDelegate InDelegate );

private:
	ConnectDelegate ConnectEvent;
	DisconnectDelegate DisconnectEvent;
	RecvDelegate RecvEvent;
	SendDelegate SendEvent;

	queue<Packet*> QueuePacketToSend;
};
