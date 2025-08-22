#include <ChatApp/Net/Packet.h>
#include <ChatApp/Client.h>
#include <iostream>
#include <thread>

using namespace ChatApp;

Client::Client(const char* ip, int port)
	: mIp(ip)
	, mPort(port)
{
	do {
		std::cout << "Username: "; std::getline(std::cin, mUsername);
	} while (!TryLogin(mUsername));
}

bool Client::TryLogin(std::string_view username)
{
	mClient = std::make_unique <SocketClient>(mIp.c_str(), mPort);
	PacketLogin login(username); login.ToNetwork();
	mClient->Send(&login, sizeof(login));
	auto packtRes = mClient->Recv();
	auto* packtBase = (ChatApp::PacketBase<>*)packtRes.data(); packtBase->ToHost();
	return packtBase->mType == ChatApp::EPacket::OK;
}

void Client::ServerMessagesThread()
{
	while (true)
	{
		auto buf = mClient->Recv();
		auto* packetMsg = (ChatApp::SvPacketMessage*)buf.data(); packetMsg->ToHost();
		std::cout << "(" << packetMsg->mUsrnm << ")" << ": " << packetMsg->mMsg << "\n";
	}
}

void Client::Run()
{
	std::thread([this] {
		ServerMessagesThread();
		}).detach();
	while (true)
	{
		std::cout << "(" << mUsername << "): "; std::string msg; std::getline(std::cin, msg);
		ChatApp::CliPacketMessage packetMsg(msg); packetMsg.ToNetwork();
		mClient->Send(&packetMsg, sizeof(packetMsg));
	}
}
