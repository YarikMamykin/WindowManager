#pragma once

namespace ymwm::environment {
	struct Handlers {
		int a;
		int b;
		Handlers(int a, int b) : a(a), b(b){}
		~Handlers() = default;
	};

}
