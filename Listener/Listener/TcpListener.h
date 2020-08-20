#pragma once

#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (49152)
class CTcpListener;

typedef void(*MessageReceivedHandler)(CTcpListener* listen, int socketId, std::string msg);


class CTcpListener {
public:
	CTcpListener(std::string ipAdress, int port, MessageReceivedHandler handler);

	//send message to client
	void Send(int clientSocket, std::string msg);


	//Intialize winsock
	bool Init();
	
	void Run();

	void CleanUp();
	//send and recieve

	//cleanup

	~CTcpListener();
private:
	//create socket
	SOCKET createSocket();
	//wait for connection
	SOCKET ConnectingClient(SOCKET listening);

	std::string m_ipAddress;
	int m_port;
	MessageReceivedHandler MessageReceived;
};