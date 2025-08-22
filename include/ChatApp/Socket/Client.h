#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <cstdint>

namespace ChatApp {
	struct SocketClient {
		SocketClient(const char* ip = "127.0.0.1", int port = 0u);
		void Send(const void* buf, size_t len);
		std::vector<std::uint8_t> Recv() const;

		SOCKET mSv;
	};
}