#define NOMINMAX

#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <cstdint>
#include <optional>

namespace ChatApp {
	struct SocketServer {

		struct Client {

			bool Send(const char* buf, size_t sz) const;
			std::optional<std::vector<std::uint8_t>> Recv() const;

			SOCKET mClient{};
			sockaddr_in mClientAddr{};
		};

		SocketServer(int port, int maxConn);
		Client WaitClient() const;
		static Client Accept(SOCKET sv);

		SOCKET mServer;
		sockaddr_in mSvAddr;
	};
}