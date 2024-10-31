#pragma once

#include <variant>

namespace ymwm::environment {
	struct Handlers;
}

namespace ymwm::commands {
	struct Cmd1 {
		void execute(environment::Handlers& handlers)const;
	};

	struct Cmd2 {
		void execute(environment::Handlers& handlers)const;
	};

	using Command = std::variant<Cmd1, Cmd2>;
}
