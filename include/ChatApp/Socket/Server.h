#define NOMINMAX

#pragma once

#include <ChatApp/Socket/Base.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <cstdint>
#include <optional>

namespace ChatApp {
	struct SocketServer {

		struct Client : SocketBase {
			sockaddr_in mClientAddr{};
		};

		SocketServer(int port, int maxConn);
		std::optional<Client> WaitClient() const;
		static std::optional<Client> Accept(SOCKET sv);

		SOCKET mServer;
		sockaddr_in mSvAddr;
	};
}