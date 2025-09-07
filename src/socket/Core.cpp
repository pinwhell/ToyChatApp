#include <ChatApp/Socket/Server.h>
#include <ChatApp/Socket/Client.h>
#include <ChatApp/Socket/Base.h>

#pragma comment(lib, "ws2_32.lib")

using namespace ChatApp;

bool SocketBase::Send(const void* buf, size_t sz) const
{
	return send(mSocket, (const char*)buf, sz, 0u) > 0;
}

std::optional<std::vector<std::uint8_t>> SocketBase::Recv() const
{
	char buf[1024];
	int bytesReceived = recv(mSocket, buf, sizeof(buf), 0);
	if (bytesReceived <= 0) return {}; // handle error or disconnection
	return std::vector<std::uint8_t>(buf, buf + bytesReceived);
}

std::optional<SocketServer::Client> SocketServer::Accept(SOCKET sv)
{
	SOCKET client{};
	sockaddr_in clientAddr{};
	int clientSize = sizeof(clientAddr);
	client = accept(sv, (sockaddr*)&clientAddr, &clientSize);
	if (client == INVALID_SOCKET) return {};
	return SocketServer::Client{ SocketBase{ client }, clientAddr };
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

std::optional<SocketServer::Client> SocketServer::WaitClient() const
{
	return Accept(mServer);
}

SocketClient::SocketClient(const char* ip, int port)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);               // Port 3000
	inet_pton(AF_INET, ip, &serverAddr.sin_addr); // Localhost
	connect(mSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
}