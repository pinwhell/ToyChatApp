#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <cstdint>
#include <optional>

namespace ChatApp {
	struct SocketClient {
		SocketClient(const char* ip = "127.0.0.1", int port = 0u);
		bool Send(const void* buf, size_t len);
		std::optional<std::vector<std::uint8_t>> Recv() const;

		SOCKET mSv;
	};
}