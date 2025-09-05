#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <optional>
#include <cstdint>
#include <vector>

namespace ChatApp {
	struct SocketBase {

		bool Send(const void* buf, size_t len) const;
		std::optional<std::vector<std::uint8_t>> Recv() const;

		SOCKET mSocket;
	};
}