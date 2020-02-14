#include "Network.h"

void Network::OnConnect( const UINT32 InSessionIndex )
{
}

void Network::OnDisconnect( const UINT32 InSessionIndex )
{
}

void Network::OnRecv( const UINT32 InSessionIndex, const WSABUF* InData )
{

}

void Network::OnSend( const UINT32 InSessionIndex )
{

}

void Network::BindConnectEvent( ConnectDelegate InDelegate )
{
	ConnectEvent = InDelegate;
}

void Network::BindDisconnectEvenet( DisconnectDelegate InDelegate )
{
	DisconnectEvent = InDelegate;
}

void Network::BindRecvEvent( RecvDelegate InDelegate )
{
	RecvEvent = InDelegate;
}

void Network::BindSendEvent( SendDelegate InDelegate )
{
	SendEvent = InDelegate;
}
