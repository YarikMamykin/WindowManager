#pragma once
#include <functional>

namespace ymwm::window::layouts {
	using Layout = std::function<void()>;

	void horizontal_stack();
	void vertical_stack();
	void grid();
	void simple();
}
