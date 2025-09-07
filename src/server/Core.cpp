#include <ChatApp/Net/Packet.h>
#include <ChatApp/Server.h>
#include <iostream>
#include <thread>

using namespace ChatApp;

bool Server::Client::Message(std::string_view byWho, std::string_view what)
{
	SvPacketMessage packt(byWho, what);
	return mClient.Send((const char*)&packt, sizeof(packt));
}

Server::Server(int port, int maxClients)
	: mServer(port, maxClients)
{}
std::optional<Server::Client*> Server::WaitClient()
{
	auto client = mServer.WaitClient();
	if (!client) return {};
	auto loginBuf = client->Recv();
	if (!loginBuf) return {};
	auto* packetBase = (PacketBase<>*)loginBuf->data();
	if (packetBase->mType != EPacket::LOGIN) {
		PacketBase<EPacket::NIL> nil;
		client->Send((const char*)&nil, sizeof(nil));
		return {};
	}
	auto* loginPacketBase = (PacketLogin*)packetBase;
	std::lock_guard lck(mClientsMtx);
	auto it = mClients.find(loginPacketBase->mUsername);
	if (it != mClients.end()) {
		PacketBase<EPacket::NIL> nil;
		client->Send((const char*)&nil, sizeof(nil));
		return {};
	}
	mClients.insert({
		std::string(loginPacketBase->mUsername),
		std::make_unique<Client>(Client{
			loginPacketBase->mUsername ,
			*client })
		});
	PacketBase<EPacket::OK> ok;
	if (!client->Send((const char*)&ok, sizeof(ok)))
	{
		mClients.erase(loginPacketBase->mUsername);
		return {};
	}
	return mClients[loginPacketBase->mUsername].get();
}
void Server::ClientThread(Client* client)
{
	std::cout << client->mUsername << ": Joined\n";
	while (true)
	{
		auto buf = client->mClient.Recv();
		if (!buf) break;
		auto* packetBase = (::ChatApp::PacketBase<>*)buf->data();
		if (packetBase->mType != EPacket::CLI_MESSAGE) {
			PacketBase<EPacket::NIL> nil;
			client->mClient.Send((const char*)&nil, sizeof(nil));
			continue;
		}
		auto* cliMsg = (CliPacketMessage*)packetBase;
		std::cout << "(" << client->mUsername << "): " << cliMsg->c_str() << "\n";
		std::lock_guard lck(mClientsMtx);
		for (auto& [key, value] : mClients)
			if (key != client->mUsername)
				value->Message(client->mUsername, cliMsg->c_str());
	}
	std::cout << client->mUsername << ": Disconnected\n";
	std::lock_guard lck(mClientsMtx);
	mClients.erase(client->mUsername);
}

void Server::Run()
{
	while (true)
	{
		auto client_ = WaitClient();
		if (!client_) continue;
		auto* chatClient = *client_;
		std::thread([this, chatClient] {
			ClientThread(chatClient);
			}).detach();
	}
}