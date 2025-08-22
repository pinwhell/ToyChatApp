#pragma once

#include <ChatApp/Socket/Client.h>
#include <ChatApp/Consts.h>
#include <string>
#include <memory>


namespace ChatApp {
	struct Client {
		Client(const char* ip = "127.0.0.1", int port = DEF_PORT);
		bool TryLogin(std::string_view username);
		void ServerMessagesThread();
		void Run();

		std::string mIp;
		int mPort;
		std::unique_ptr<SocketClient> mClient;
		std::string mUsername;
	};
}