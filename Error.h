#pragma once
#include <array>

namespace ymwm::errors{
	enum class ErrorCode: unsigned int {
		NONE
	};
	static inline constexpr unsigned int MAX_ERROR_MESSAGE_LENGTH{1024u};
}

namespace ymwm::errors {
	struct Error{
		ErrorCode code;
		std::array<char, MAX_ERROR_MESSAGE_LENGTH> message;
	};
}
