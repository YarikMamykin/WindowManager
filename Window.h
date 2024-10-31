#pragma once

#include <cstdint>

namespace ymwm::window {
	struct Window {
		uintptr_t id;
		int x;
		int y;
		int w;
		int h;
	};
}
