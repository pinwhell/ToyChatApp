#define NOMINMAX

#pragma once

#include <ChatApp/Net/EPacket.h>
#include <ChatApp/Consts.h>
#include <cstdint>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma pack(push, 1)
namespace ChatApp {
	template<EPacket Type = EPacket::NIL>
	struct PacketBase {
		EPacket mType = Type;
	};

	struct PacketLogin : PacketBase<EPacket::LOGIN> {
		PacketLogin(std::string_view usrName)
		{
			memset(mUsername, 0u, sizeof(mUsername));
			strncpy(mUsername, usrName.data(), std::min(usrName.size(), MAX_USRNM_SIZE));
		}

		char mUsername[MAX_USRNM_SIZE];
	};

	struct SvPacketMessage : PacketBase<EPacket::SV_MESSAGE> {
		SvPacketMessage(std::string_view byWho, std::string_view what)
		{
			strncpy(mMsg, what.data(), std::min(what.size() + 1, MAX_MSG_SIZE));
			strncpy(mUsrnm, byWho.data(), std::min(byWho.size() + 1, MAX_USRNM_SIZE));
		}

		char mUsrnm[MAX_USRNM_SIZE];
		char mMsg[MAX_MSG_SIZE];
	};

	struct CliPacketMessage : PacketBase<EPacket::CLI_MESSAGE> {
		CliPacketMessage(std::string_view what)
		{
			strncpy(mMsg, what.data(), std::min(what.size() + 1, MAX_MSG_SIZE));
		}

		char mMsg[MAX_MSG_SIZE];
	};
}
#pragma pack(pop)