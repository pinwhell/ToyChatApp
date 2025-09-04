#pragma once

#include <ChatApp/Socket/Server.h>
#include <ChatApp/Consts.h>
#include <unordered_map>
#include <string_view>
#include <optional>
#include <memory>
#include <string>
#include <mutex>

namespace ChatApp {
	struct Server {
		struct Client {
			bool Message(std::string_view byWho, std::string_view what);

			std::string mUsername;
			SocketServer::Client mClient;
		};

		Server(int port = DEF_PORT, int maxClients = MAX_CONN);

		std::optional<Client*> WaitClient();
		void ClientThread(Client* client);
		void Run();

		SocketServer mServer;
		std::unordered_map<std::string, std::unique_ptr<Client>> mClients;
		std::mutex mClientsMtx;
	};
}