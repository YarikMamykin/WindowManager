#include "../Environment.h"
#include "./Handlers.h"
#include <iostream>
#include <memory>

namespace ymwm::environment {
	Environment::Environment() : m_handlers(std::make_unique<Handlers>(10,23.3)){}
	Environment::~Environment() = default;
	void Environment::show_handlers() {
		std::cout << m_handlers->a << " " << m_handlers->b << std::endl;
	}
}
