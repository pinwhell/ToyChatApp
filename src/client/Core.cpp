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
	PacketLogin login(username);
	if (!mClient->Send(&login, sizeof(login))) return false;
	auto packtRes = mClient->Recv();
	if (!packtRes) return false;
	auto* packtBase = (ChatApp::PacketBase<>*)packtRes->data();
	return packtBase->mType == ChatApp::EPacket::OK;
}

void Client::ServerMessagesThread()
{
	while (true)
	{
		auto buf = mClient->Recv();
		if (!buf) break;
		auto* packetMsg = (ChatApp::SvPacketMessage*)buf->data();
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
		ChatApp::CliPacketMessage packetMsg(msg);
		if (!mClient->Send(&packetMsg, sizeof(packetMsg))) break;
	}
}
