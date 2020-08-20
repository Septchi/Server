#include <iostream>
#include<string>

#include "TcpListener.h"

void Listener_MessageRecieved(CTcpListener* listener, int client, std::string msg);

void main()
{
	CTcpListener server("192.168.0.22", 54010, Listener_MessageRecieved);
	if (server.Init())
	{
		server.Run();
	}
}

void Listener_MessageRecieved(CTcpListener* listener, int client, std::string msg)
{
	listener->Send(client, msg);
}
