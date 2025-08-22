#pragma once

#include <cstdint>

namespace ChatApp {
	enum class EPacket : std::uint32_t {
		NIL = -1,
		LOGIN,
		SV_MESSAGE,
		CLI_MESSAGE,
		OK
	};
}