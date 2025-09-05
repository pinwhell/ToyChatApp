#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ChatApp/Socket/Base.h>
#include <optional>
#include <cstdint>
#include <vector>
#include <memory>

namespace ChatApp {
	struct SocketClient : SocketBase {
		SocketClient(const char* ip = "127.0.0.1", int port = 0u);
	};
}