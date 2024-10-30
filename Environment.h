#pragma once

#include <memory>
namespace ymwm::environment {
	struct Environment {
		Environment();
		~Environment();
		void show_handlers();
		private:
			struct Handlers;
			std::unique_ptr<Handlers> m_handlers;
	};
}
