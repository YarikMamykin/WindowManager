#pragma once

namespace ymwm::environment {
	struct Handlers {
		int a;
		double b;
		Handlers(int a, double b) : a(a), b(b){}
		~Handlers() = default;
	};

}
