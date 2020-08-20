#include "TcpListener.h"

CTcpListener::CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler)
	:m_ipAddress(ipAddress), m_port(port), MessageReceived(handler)
{

}


CTcpListener::~CTcpListener()
{
	CTcpListener::CleanUp();
}


//send message to client
void CTcpListener::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}


//Intialize winsock
bool CTcpListener::Init()
{
	WSAData data;
	WORD ver = MAKEWORD(2,2);

	int WsCheck = WSAStartup(ver, &data);
	
	return WsCheck == 0;
}

void CTcpListener::Run()
{
	char buf[MAX_BUFFER_SIZE];

	ZeroMemory(buf, MAX_BUFFER_SIZE);
	while (true)
	{
		//create a listening socket
		SOCKET listening = createSocket();
		if (listening == INVALID_SOCKET) break;

		fd_set master;
		FD_ZERO(&master);
		FD_SET(listening, &master);


		while (true)
		{
			fd_set copy = master;
			int sockCount = select(0, &copy, nullptr, nullptr, nullptr);
			for (int i = 0; i < sockCount; i++)
			{
				SOCKET sock = copy.fd_array[i];
				if (sock == listening)
				{
					//accepting client
					SOCKET client = accept(listening, nullptr, nullptr);

					//add client to list
					FD_SET(client, &master);
					//welcome message
					std::string msg = "hell0\r\n";
					Send(client, msg);

				}
				else
				{
					int bytesRecv =  recv(sock, buf, MAX_BUFFER_SIZE, 0);
					if (bytesRecv <= 0)
					{
						//close socket
						closesocket(sock);
						FD_CLR(sock, &master);
					}
					else
					{
						for (int i = 0; i < master.fd_count; i++)
						{
							//send to other clients and not listening socket
							SOCKET outsock = master.fd_array[i];
							if (outsock != listening && outsock != sock)
							{
								MessageReceived(this, outsock, std::string(buf, 0, bytesRecv));
							}
						}
					}

				}

			}
		}
	}
}

void CTcpListener::CleanUp()
{

}
//send and recieve

//cleanup


//create socket
SOCKET CTcpListener::createSocket()
{
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening != INVALID_SOCKET)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

		int bindCheck = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindCheck != SOCKET_ERROR)
		{
			int listeningCheck = listen(listening, SOMAXCONN);
			if (listeningCheck == SOCKET_ERROR)
			{
				return -1;
			}
		}
		else return -1;
		
	}
	return listening;
}
//wait for connection
SOCKET CTcpListener::ConnectingClient(SOCKET listening)
{
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	return clientSocket;
}

