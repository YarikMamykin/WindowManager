#include "../Environment.h"
#include <iostream>
#include <memory>

namespace ymwm::environment {
	struct Environment::Handlers {
		int a;
		int b;
		Handlers(int a, int b) : a(a), b(b){}
		~Handlers() = default;
	};

	Environment::Environment() : m_handlers(std::make_unique<Handlers>(10,23)){}
	Environment::~Environment() = default;
	void Environment::show_handlers() {
		std::cout << m_handlers->a << " " << m_handlers->b << std::endl;
	}
}

