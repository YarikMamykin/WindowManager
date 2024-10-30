#include "../Environment.h"
#include <iostream>
#include <memory>

namespace ymwm::environment {
	struct Environment::Handlers {
		int a;
		double b;
		Handlers(int a, double b) : a(a), b(b){}
		~Handlers() = default;
	};

	Environment::Environment() : m_handlers(std::make_unique<Handlers>(10,23.3)){}
	Environment::~Environment() = default;
	void Environment::show_handlers() {
		std::cout << m_handlers->a << " " << m_handlers->b << std::endl;
	}
}
