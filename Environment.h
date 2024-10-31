#pragma once

#include <memory>
#include <variant>
#include "Command.h"
#include "Handlers.h"

namespace ymwm::environment {
	struct Environment {
		Environment();
		~Environment();
		void show_handlers();
		inline void execute(const commands::Command& cmd){
			std::visit([&handlers=*m_handlers](auto c){
					c.execute(handlers);
					}, cmd);
		}
		private:
			std::unique_ptr<Handlers> m_handlers;
	};
}
