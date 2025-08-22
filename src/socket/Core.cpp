#include <ChatApp/Socket/Server.h>
#include <ChatApp/Socket/Client.h>

#pragma comment(lib, "ws2_32.lib")

using namespace ChatApp;

void SocketServer::Client::Send(const char* buf, size_t sz) const
{
	send(mClient, buf, sz, 0u);
}

std::vector<std::uint8_t> SocketServer::Client::Recv() const
{
	char buf[1024];
	int bytesReceived = recv(mClient, buf, sizeof(buf), 0);
	if (bytesReceived <= 0) return {}; // handle error or disconnection
	return std::vector<std::uint8_t>(buf, buf + bytesReceived);
}

SocketServer::Client SocketServer::Accept(SOCKET sv)
{
	SOCKET client{};
	sockaddr_in clientAddr{};
	int clientSize = sizeof(clientAddr);
	client = accept(sv, (sockaddr*)&clientAddr, &clientSize);
	return SocketServer::Client{ client, clientAddr };
}

SocketServer::SocketServer(int port, int maxConn)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	mServer = socket(AF_INET, SOCK_STREAM, 0);
	mSvAddr.sin_family = AF_INET;
	mSvAddr.sin_addr.s_addr = INADDR_ANY; // bind to all interfaces
	mSvAddr.sin_port = htons(port);       // example port 5000
	bind(mServer, (sockaddr*)&mSvAddr, sizeof(mSvAddr));
	listen(mServer, maxConn);
}

SocketServer::Client SocketServer::WaitClient() const
{
	return Accept(mServer);
}

SocketClient::SocketClient(const char* ip, int port)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	mSv = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);               // Port 3000
	inet_pton(AF_INET, ip, &serverAddr.sin_addr); // Localhost
	connect(mSv, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

void SocketClient::Send(const void* buf, size_t len)
{
	send(mSv, (const char*)buf, len, 0u);
}

std::vector<std::uint8_t> SocketClient::Recv() const
{
	char buf[1024]{};
	int bytesReceived = recv(mSv, buf, sizeof(buf), 0);
	if (bytesReceived <= 0) return {}; // handle error or disconnection
	return std::vector<std::uint8_t>(buf, buf + bytesReceived);
}