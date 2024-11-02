#pragma once
#include "EnvironmentID.h"

namespace ymwm::window {
	struct Window {
		EnvironmentID id;
		EnvironmentID group;
		EnvironmentID monitor;
		int x;
		int y;
		int w;
		int h;
	};
}
