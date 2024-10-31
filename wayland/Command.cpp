
#include "../Command.h"
#include "./Handlers.h"
#include <iostream>

namespace ymwm::commands {
	void Cmd1::execute(environment::Handlers& handlers) const {

		std::cout << "CMD 1: " << handlers.a << " " << handlers.b << std::endl;
	}

	void Cmd2::execute(environment::Handlers& handlers)const  {
		std::cout << "CMD 2: " << handlers.a << " " << handlers.b << std::endl;
	}
}
