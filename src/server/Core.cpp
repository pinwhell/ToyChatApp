#include <ChatApp/Net/Packet.h>
#include <ChatApp/Server.h>
#include <iostream>
#include <thread>

using namespace ChatApp;

void Server::Client::Message(std::string_view byWho, std::string_view what)
{
	SvPacketMessage packt(byWho, what); packt.ToNetwork();
	mClient.Send((const char*)&packt, sizeof(packt));
}

Server::Server(int port, int maxClients)
	: mServer(port, maxClients)
{}
std::optional<Server::Client*> Server::WaitClient()
{
	auto client = mServer.WaitClient();
	auto loginBuf = client.Recv();
	auto* packetBase = (PacketBase<>*)loginBuf.data();
	packetBase->ToHost();
	if (packetBase->mType != EPacket::LOGIN) {
		::ChatApp::PacketBase<EPacket::NIL> nil; nil.ToNetwork();
		client.Send((const char*)&nil, sizeof(nil));
		return {};
	}
	auto* loginPacketBase = (PacketLogin*)packetBase;
	std::lock_guard lck(mClientsMtx);
	auto it = mClients.find(loginPacketBase->mUsername);
	if (it != mClients.end()) {
		::ChatApp::PacketBase<EPacket::NIL> nil; nil.ToNetwork();
		client.Send((const char*)&nil, sizeof(nil));
		return {};
	}
	auto pair = mClients.insert({
		std::string(loginPacketBase->mUsername),
		std::make_unique<Client>(Client{
			loginPacketBase->mUsername ,
			client })
		});
	::ChatApp::PacketBase<EPacket::OK> ok; ok.ToNetwork();
	client.Send((const char*)&ok, sizeof(ok));
	return mClients[loginPacketBase->mUsername].get();
}
void Server::ClientThread(Client* client)
{
	std::cout << client->mUsername << ": Joined\n";
	while (true)
	{
		auto buf = client->mClient.Recv();
		auto* packetBase = (::ChatApp::PacketBase<>*)buf.data();
		packetBase->ToHost();
		if (packetBase->mType != EPacket::CLI_MESSAGE) continue;
		auto* cliMsg = (CliPacketMessage*)packetBase;
		std::cout << "(" << client->mUsername << "): " << cliMsg->mMsg << "\n";
		std::lock_guard lck(mClientsMtx);
		for (auto& [key, value] : mClients)
			if (key != client->mUsername)
				value->Message(client->mUsername, cliMsg->mMsg);

	}
}

void Server::Run()
{
	while (true)
	{
		auto client_ = WaitClient();
		if (!client_) continue;
		auto& chatClient = **client_;
		std::thread([this, &client_] {
			ClientThread(*client_);
			}).detach();
	}
}